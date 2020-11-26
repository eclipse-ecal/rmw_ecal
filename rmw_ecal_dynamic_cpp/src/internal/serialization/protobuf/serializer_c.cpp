/* ========================= RMW eCAL LICENSE =================================
 *
 * Copyright (C) 2019 - 2020 Continental Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * ========================= RMW eCAL LICENSE =================================
*/

#include "serializer_c.hpp"

#include <string>
#include <memory>
#include <stdexcept>

#include <rosidl_typesupport_introspection_c/field_types.h>

#include "internal/rosidl_generator_c_pkg_adapter.hpp"
#include "internal/common.hpp"
#include "internal/serialization/protobuf/helpers.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4267 4244)
#endif

namespace eCAL
{
  namespace rmw
  {

    namespace pb = google::protobuf;

    using MessageMembers = rosidl_typesupport_introspection_c__MessageMembers;

#define DEFINE_SET_METHODS(PB_NAME, TYPE)                                                   \
    template <>                                                                             \
    void CProtobufSerializer::SetSingle<TYPE>(const char *data, pb::Message *msg,           \
                                              const pb::FieldDescriptor *field) const       \
    {                                                                                       \
      auto ref = msg->GetReflection();                                                      \
      auto value = *reinterpret_cast<const TYPE *>(data);                                   \
                                                                                            \
      ref->Set##PB_NAME(msg, field, value);                                                 \
    }                                                                                       \
                                                                                            \
    template <>                                                                             \
    void CProtobufSerializer::SetArray<TYPE>(const char *data, int size, pb::Message *msg,  \
                                             const pb::FieldDescriptor *field) const        \
    {                                                                                       \
      auto ref = msg->GetReflection();                                                      \
      auto array = reinterpret_cast<const TYPE *>(data);                                    \
      for (int i = 0; i < size; i++)                                                        \
      {                                                                                     \
        ref->Add##PB_NAME(msg, field, array[i]);                                            \
      }                                                                                     \
    }                                                                                       \
                                                                                            \
    template <>                                                                             \
    void CProtobufSerializer::SetDynamicArray<TYPE>(const char *data, pb::Message *msg,     \
                                                    const pb::FieldDescriptor *field) const \
    {                                                                                       \
      auto ref = msg->GetReflection();                                                      \
      auto sequence = reinterpret_cast<const rosidl_runtime_c__char__Sequence *>(data);     \
                                                                                            \
      ThrowIfInvalidProtobufArraySize(sequence->size);                                      \
      for (size_t i = 0; i < sequence->size; i++)                                           \
      {                                                                                     \
        auto value = reinterpret_cast<TYPE *>(sequence->data + sizeof(TYPE) * i);           \
        ref->Add##PB_NAME(msg, field, *value);                                              \
      }                                                                                     \
    }

    DEFINE_SET_METHODS(Bool, bool)
    DEFINE_SET_METHODS(Int32, char)
    DEFINE_SET_METHODS(Int32, int8_t)
    DEFINE_SET_METHODS(Int32, int16_t)
    DEFINE_SET_METHODS(Int32, int32_t)
    DEFINE_SET_METHODS(Int64, int64_t)
    DEFINE_SET_METHODS(UInt32, uint8_t)
    DEFINE_SET_METHODS(UInt32, uint16_t)
    DEFINE_SET_METHODS(UInt32, uint32_t)
    DEFINE_SET_METHODS(UInt64, uint64_t)
    DEFINE_SET_METHODS(Float, float)
    DEFINE_SET_METHODS(Double, double)

    template <>
    void CProtobufSerializer::SetSingle<std::string>(const char* data, pb::Message* msg,
                                                     const pb::FieldDescriptor* field) const
    {
      auto ref = msg->GetReflection();
      auto sequence = reinterpret_cast<const rosidl_runtime_c__char__Sequence*>(data);
      auto str_data = reinterpret_cast<char*>(sequence->data);

      ref->SetString(msg, field, str_data);
    }

    template <>
    void CProtobufSerializer::SetArray<std::string>(const char* data, int size, pb::Message* msg,
                                                    const pb::FieldDescriptor* field) const
    {
      auto ref = msg->GetReflection();
      auto array = reinterpret_cast<const rosidl_runtime_c__char__Sequence*>(data);

      for (int i = 0; i < size; i++)
      {
        auto& string = array[i];
        auto str_data = reinterpret_cast<char*>(string.data);
        ref->AddString(msg, field, str_data);
      }
    }

    template <>
    void CProtobufSerializer::SetDynamicArray<std::string>(const char* data, pb::Message* msg,
                                                           const pb::FieldDescriptor* field) const
    {
      auto ref = msg->GetReflection();
      auto sequence = reinterpret_cast<const rosidl_runtime_c__char__Sequence*>(data);
      auto size = sequence->size;
      auto strings = sequence->data;

      ThrowIfInvalidProtobufArraySize(size);

      for (size_t i = 0; i < size; i++, strings += sizeof(rosidl_runtime_c__char__Sequence))
      {
        auto string = reinterpret_cast<const rosidl_runtime_c__char__Sequence*>(strings);
        auto str_data = reinterpret_cast<char*>(string->data);

        ref->AddString(msg, field, str_data);
      }
    }

    template <>
    void CProtobufSerializer::SetSingle<ros_message_t>(const char* data, const MessageMembers* members,
                                                       pb::Message* msg, const pb::FieldDescriptor* field) const
    {
      auto sub_message = message_factory_.Create(members->message_name_);
      auto sub_desc = sub_message->GetDescriptor();
      FillMessage(data, members, sub_message, sub_desc);

      auto ref = msg->GetReflection();
      ref->SetAllocatedMessage(msg, sub_message, field);
    }

    template <>
    void CProtobufSerializer::SetArray<ros_message_t>(const char* data, const MessageMembers* members,
                                                      int size, pb::Message* msg,
                                                      const pb::FieldDescriptor* field) const
    {
      auto ref = msg->GetReflection();

      for (int i = 0; i < size; i++)
      {
        auto sub_message = message_factory_.Create(members->message_name_);
        auto sub_desc = sub_message->GetDescriptor();
        FillMessage(data, members, sub_message, sub_desc);

        ref->AddAllocatedMessage(msg, field, sub_message);

        data += members->size_of_;
      }
    }

    template <>
    void CProtobufSerializer::SetDynamicArray<ros_message_t>(const char* data, const MessageMembers* members,
                                                             pb::Message* msg, const pb::FieldDescriptor* field) const
    {
      auto sequence = reinterpret_cast<const rosidl_runtime_c__char__Sequence*>(data);

      auto arr_data = reinterpret_cast<char*>(sequence->data);
      auto arr_size = sequence->size;
      ThrowIfInvalidProtobufArraySize(arr_size);
      SetArray<ros_message_t>(arr_data, members, arr_size, msg, field);
    }

    template <typename T>
    void CProtobufSerializer::Set(const char* data,
                                  const rosidl_typesupport_introspection_c__MessageMember* member,
                                  pb::Message* msg, const pb::Descriptor* desc) const
    {
      auto pb_field = desc->FindFieldByName(member->name_);

      if (member->is_array_)
      {
        if (member->array_size_ > 0 && !member->is_upper_bound_)
        {
          ThrowIfInvalidProtobufArraySize(member->array_size_);
          SetArray<T>(data, member->array_size_, msg, pb_field);
        }
        else
        {
          SetDynamicArray<T>(data, msg, pb_field);
        }
      }
      else
      {
        SetSingle<T>(data, msg, pb_field);
      }
    }

    template <>
    void CProtobufSerializer::Set<ros_message_t>(const char* data,
                                                 const rosidl_typesupport_introspection_c__MessageMember* member,
                                                 pb::Message* msg, const pb::Descriptor* desc) const
    {
      auto pb_field = desc->FindFieldByName(member->name_);
      auto sub_members = GetMembers(member);

      if (member->is_array_)
      {
        if (member->array_size_ > 0 && !member->is_upper_bound_)
        {
          ThrowIfInvalidProtobufArraySize(member->array_size_);
          SetArray<ros_message_t>(data, sub_members, member->array_size_, msg, pb_field);
        }
        else
        {
          SetDynamicArray<ros_message_t>(data, sub_members, msg, pb_field);
        }
      }
      else
      {
        SetSingle<ros_message_t>(data, sub_members, msg, pb_field);
      }
    }

#undef DEFINE_SET_METHODS

    void CProtobufSerializer::FillMessage(const char* data, const MessageMembers* members,
                                          pb::Message* msg, const pb::Descriptor* desc) const
    {
      for (uint32_t i = 0; i < members->member_count_; i++)
      {
        auto member = members->members_ + i;
        auto member_data = data + member->offset_;

        switch (member->type_id_)
        {
        case ::rosidl_typesupport_introspection_c__ROS_TYPE_STRING:
          Set<std::string>(member_data, member, msg, desc);
          break;
        case ::rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN:
          Set<bool>(member_data, member, msg, desc);
          break;
        case ::rosidl_typesupport_introspection_c__ROS_TYPE_BYTE:
          Set<uint8_t>(member_data, member, msg, desc);
          break;
        case ::rosidl_typesupport_introspection_c__ROS_TYPE_CHAR:
          Set<char>(member_data, member, msg, desc);
          break;
        case ::rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT:
          Set<float>(member_data, member, msg, desc);
          break;
        case ::rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE:
          Set<double>(member_data, member, msg, desc);
          break;
        case ::rosidl_typesupport_introspection_c__ROS_TYPE_INT8:
          Set<int8_t>(member_data, member, msg, desc);
          break;
        case ::rosidl_typesupport_introspection_c__ROS_TYPE_INT16:
          Set<int16_t>(member_data, member, msg, desc);
          break;
        case ::rosidl_typesupport_introspection_c__ROS_TYPE_INT32:
          Set<int32_t>(member_data, member, msg, desc);
          break;
        case ::rosidl_typesupport_introspection_c__ROS_TYPE_INT64:
          Set<int64_t>(member_data, member, msg, desc);
          break;
        case ::rosidl_typesupport_introspection_c__ROS_TYPE_UINT8:
          Set<uint8_t>(member_data, member, msg, desc);
          break;
        case ::rosidl_typesupport_introspection_c__ROS_TYPE_UINT16:
          Set<uint16_t>(member_data, member, msg, desc);
          break;
        case ::rosidl_typesupport_introspection_c__ROS_TYPE_UINT32:
          Set<uint32_t>(member_data, member, msg, desc);
          break;
        case ::rosidl_typesupport_introspection_c__ROS_TYPE_UINT64:
          Set<uint64_t>(member_data, member, msg, desc);
          break;
        case ::rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE:
          Set<ros_message_t>(member_data, member, msg, desc);
          break;
        case ::rosidl_typesupport_introspection_c__ROS_TYPE_LONG_DOUBLE:
        case ::rosidl_typesupport_introspection_c__ROS_TYPE_WSTRING:
        case ::rosidl_typesupport_introspection_c__ROS_TYPE_WCHAR:
          throw std::logic_error("Wide character/string serialization is unsupported.");
        }
      }
    }

    const std::string CProtobufSerializer::Serialize(const void* data)
    {
      std::unique_ptr<pb::Message> msg(message_factory_.Create());
      auto msg_desc = msg->GetDescriptor();
      FillMessage(static_cast<const char*>(data), members_, msg.get(), msg_desc);
      return msg->SerializeAsString();
    }

    const std::string CProtobufSerializer::GetMessageStringDescriptor() const
    {
      return message_factory_.GetMessageStringDescriptor();
    }

  } // namespace rmw
} // namespace eCAL

#ifdef _MSC_VER
#pragma warning(pop)
#endif
