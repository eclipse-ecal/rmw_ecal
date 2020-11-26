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

#include "deserializer_c.hpp"

#include <string>
#include <stdexcept>
#include <algorithm>
#include <memory>

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

    #define DEFINE_SET_METHODS(PB_NAME, TYPE)                                                 \
    template <>                                                                               \
    void CProtobufDeserializer::SetSingle<TYPE>(char *ros_member,                             \
                                                const pb::Message *msg,                       \
                                                const pb::FieldDescriptor *field) const       \
    {                                                                                         \
      auto data = reinterpret_cast<TYPE *>(ros_member);                                       \
      auto ref = msg->GetReflection();                                                        \
      *data = ref->Get##PB_NAME(*msg, field);                                                 \
    }                                                                                         \
                                                                                              \
    template <>                                                                               \
    void CProtobufDeserializer::SetArray<TYPE>(char *ros_member,                              \
                                               int array_size,                                \
                                               const pb::Message *msg,                        \
                                               const pb::FieldDescriptor *field) const        \
    {                                                                                         \
      auto data = reinterpret_cast<TYPE *>(ros_member);                                       \
      auto ref = msg->GetReflection();                                                        \
      for (int i = 0; i < array_size; i++, data++)                                            \
      {                                                                                       \
        *data = ref->GetRepeated##PB_NAME(*msg, field, i);                                    \
      }                                                                                       \
    }                                                                                         \
                                                                                              \
    template <>                                                                               \
    void CProtobufDeserializer::SetDynamicArray<TYPE>(char *ros_member,                       \
                                                      const pb::Message *msg,                 \
                                                      const pb::FieldDescriptor *field) const \
    {                                                                                         \
      auto sequence = reinterpret_cast<rosidl_runtime_c__octet__Sequence *>(ros_member);      \
      auto ref = msg->GetReflection();                                                        \
      auto size = ref->FieldSize(*msg, field);                                                \
                                                                                              \
      sequence->size = size;                                                                  \
      sequence->capacity = size;                                                              \
      if (size > 0)                                                                           \
      {                                                                                       \
        sequence->data = new uint8_t[size * sizeof(TYPE)];                                    \
        SetArray<TYPE>(reinterpret_cast<char *>(sequence->data),                              \
                         sequence->size, msg, field);                                         \
      }                                                                                       \
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
    void CProtobufDeserializer::SetSingle<std::string>(char* ros_member,
                                                       const pb::Message* msg,
                                                       const pb::FieldDescriptor* field) const
    {
      auto data = reinterpret_cast<rosidl_runtime_c__char__Sequence*>(ros_member);
      auto ref = msg->GetReflection();

      const std::string& str = ref->GetStringReference(*msg, field, nullptr);
      auto size = str.size();
      data->size = size;
      data->capacity = size + 1;
      data->data = new signed char[data->capacity];
      auto str_data = reinterpret_cast<const signed char*>(str.c_str());

      std::copy_n(str_data, size + 1, data->data);
    }

    template <>
    void CProtobufDeserializer::SetArray<std::string>(char* ros_member,
                                                      int array_size,
                                                      const pb::Message* msg,
                                                      const pb::FieldDescriptor* field) const
    {
      auto sequence = reinterpret_cast<rosidl_runtime_c__char__Sequence*>(ros_member);
      auto ref = msg->GetReflection();

      for (int i = 0; i < array_size; i++)
      {
        auto data = reinterpret_cast<rosidl_runtime_c__char__Sequence*>(&sequence[i]);
        const std::string& str = ref->GetRepeatedStringReference(*msg, field, i, nullptr);
        auto size = str.size();
        data->size = size;
        data->capacity = size + 1;
        data->data = new signed char[data->capacity];
        auto str_data = reinterpret_cast<const signed char*>(str.c_str());

        std::copy_n(str_data, size + 1, data->data);
      }
    }

    template <>
    void CProtobufDeserializer::SetDynamicArray<std::string>(char* ros_member,
                                                             const pb::Message* msg,
                                                             const pb::FieldDescriptor* field) const
    {
      auto sequence = reinterpret_cast<rosidl_runtime_c__octet__Sequence*>(ros_member);

      auto ref = msg->GetReflection();
      auto array_size = ref->FieldSize(*msg, field);
      sequence->size = array_size;
      sequence->capacity = array_size;
      if (array_size > 0)
      {
        auto sequence_data = new char[array_size * sizeof(rosidl_runtime_c__char__Sequence)];
        SetArray<std::string>(sequence_data, array_size, msg, field);
        sequence->data = reinterpret_cast<uint8_t*>(sequence_data);
      }
    }

    template <>
    void CProtobufDeserializer::SetSingle<ros_message_t>(char* ros_member,
                                                         const rosidl_typesupport_introspection_c__MessageMembers* members,
                                                         const pb::Message* msg,
                                                         const pb::FieldDescriptor* field) const
    {
      auto ref = msg->GetReflection();
      auto& sub_message = ref->GetMessage(*msg, field);
      FillMessage(ros_member, members, &sub_message);
    }

    template <>
    void CProtobufDeserializer::SetArray<ros_message_t>(char* ros_member,
                                                        const rosidl_typesupport_introspection_c__MessageMembers* members,
                                                        int size,
                                                        const pb::Message* msg,
                                                        const pb::FieldDescriptor* field) const
    {
      auto ref = msg->GetReflection();
      for (int i = 0; i < size; i++)
      {
        auto& sub_message = ref->GetRepeatedMessage(*msg, field, i);
        FillMessage(ros_member, members, &sub_message);
        ros_member += members->size_of_;
      }
    }

    template <>
    void CProtobufDeserializer::SetDynamicArray<ros_message_t>(char* ros_member,
                                                               const rosidl_typesupport_introspection_c__MessageMembers* members,
                                                               const pb::Message* msg,
                                                               const pb::FieldDescriptor* field) const
    {
      auto sequence = reinterpret_cast<rosidl_runtime_c__octet__Sequence*>(ros_member);

      auto ref = msg->GetReflection();
      auto array_size = ref->FieldSize(*msg, field);
      sequence->size = array_size;
      sequence->capacity = array_size;
      if (array_size > 0)
      {
        auto sequence_data = new char[array_size * members->size_of_];
        SetArray<ros_message_t>(sequence_data, members, array_size, msg, field);
        sequence->data = reinterpret_cast<uint8_t*>(sequence_data);
      }
    }

  #undef DEFINE_SET_METHODS

    template <typename T>
    void CProtobufDeserializer::Set(char* ros_member,
                                    const rosidl_typesupport_introspection_c__MessageMember* member,
                                    const pb::Message* msg,
                                    const pb::FieldDescriptor* field) const
    {
      if (member->is_array_)
      {
        if (member->array_size_ > 0 && !member->is_upper_bound_)
        {
          ThrowIfInvalidProtobufArraySize(member->array_size_);
          SetArray<T>(ros_member, member->array_size_, msg, field);
        }
        else //dynamic array
        {
          SetDynamicArray<T>(ros_member, msg, field);
        }
      }
      else
      {
        SetSingle<T>(ros_member, msg, field);
      }
    }

    template <>
    void CProtobufDeserializer::Set<ros_message_t>(char* ros_member,
                                                   const rosidl_typesupport_introspection_c__MessageMember* member,
                                                   const pb::Message* msg,
                                                   const pb::FieldDescriptor* field) const
    {
      auto members = GetMembers(member);
      if (member->is_array_)
      {
        if (member->array_size_ > 0 && !member->is_upper_bound_)
        {
          ThrowIfInvalidProtobufArraySize(member->array_size_);
          SetArray<ros_message_t>(ros_member, members, member->array_size_, msg, field);
        }
        else //dynamic array
        {
          SetDynamicArray<ros_message_t>(ros_member, members, msg, field);
        }
      }
      else
      {
        SetSingle<ros_message_t>(ros_member, members, msg, field);
      }
    }

    void CProtobufDeserializer::FillMessage(char* ros_msg,
                                            const rosidl_typesupport_introspection_c__MessageMembers* members,
                                            const pb::Message* msg) const
    {
      auto msg_desc = msg->GetDescriptor();
      for (size_t i = 0; i < members->member_count_; i++)
      {
        auto member = members->members_ + i;
        auto member_data = ros_msg + member->offset_;

        auto field = msg_desc->FindFieldByName(member->name_);

        switch (member->type_id_)
        {
        case rosidl_typesupport_introspection_c__ROS_TYPE_STRING:
          Set<std::string>(member_data, member, msg, field);
          break;
        case rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN:
          Set<bool>(member_data, member, msg, field);
          break;
        case rosidl_typesupport_introspection_c__ROS_TYPE_BYTE:
          Set<uint8_t>(member_data, member, msg, field);
          break;
        case rosidl_typesupport_introspection_c__ROS_TYPE_CHAR:
          Set<char>(member_data, member, msg, field);
          break;
        case rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT:
          Set<float>(member_data, member, msg, field);
          break;
        case rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE:
          Set<double>(member_data, member, msg, field);
          break;
        case rosidl_typesupport_introspection_c__ROS_TYPE_INT8:
          Set<int8_t>(member_data, member, msg, field);
          break;
        case rosidl_typesupport_introspection_c__ROS_TYPE_INT16:
          Set<int16_t>(member_data, member, msg, field);
          break;
        case rosidl_typesupport_introspection_c__ROS_TYPE_INT32:
          Set<int32_t>(member_data, member, msg, field);
          break;
        case rosidl_typesupport_introspection_c__ROS_TYPE_INT64:
          Set<int64_t>(member_data, member, msg, field);
          break;
        case rosidl_typesupport_introspection_c__ROS_TYPE_UINT8:
          Set<uint8_t>(member_data, member, msg, field);
          break;
        case rosidl_typesupport_introspection_c__ROS_TYPE_UINT16:
          Set<uint16_t>(member_data, member, msg, field);
          break;
        case rosidl_typesupport_introspection_c__ROS_TYPE_UINT32:
          Set<uint32_t>(member_data, member, msg, field);
          break;
        case rosidl_typesupport_introspection_c__ROS_TYPE_UINT64:
          Set<uint64_t>(member_data, member, msg, field);
          break;
        case rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE:
          Set<ros_message_t>(member_data, member, msg, field);
          break;
        case rosidl_typesupport_introspection_c__ROS_TYPE_WSTRING:
        case rosidl_typesupport_introspection_c__ROS_TYPE_WCHAR:
          throw std::logic_error("Wide character/string deserialization is unsupported.");
        }
      }
    }

    void CProtobufDeserializer::Deserialize(void* message, const void* serialized_data, size_t size)
    {
      std::unique_ptr<pb::Message> proto_message(message_factory_.Create());
      proto_message->ParseFromArray(serialized_data, size);
      auto message_bytes = static_cast<char*>(message);
      FillMessage(message_bytes, members_, proto_message.get());
    }

  } // namespace rmw
} // namespace eCAL

#ifdef _MSC_VER
#pragma warning(pop)
#endif
