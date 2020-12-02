// Copyright 2020 Continental AG
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "serializer_cpp.hpp"

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

#include <rosidl_typesupport_introspection_cpp/field_types.hpp>

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
    namespace ts_introspection = rosidl_typesupport_introspection_cpp;

#define DEFINE_SET_METHODS(PB_NAME, TYPE)                                                     \
    template <>                                                                               \
    void CppProtobufSerializer::SetSingle<TYPE>(const char *data, pb::Message *msg,           \
                                                const pb::FieldDescriptor *field) const       \
    {                                                                                         \
      auto ref = msg->GetReflection();                                                        \
      auto value = *reinterpret_cast<const TYPE *>(data);                                     \
                                                                                              \
      ref->Set##PB_NAME(msg, field, value);                                                   \
    }                                                                                         \
                                                                                              \
    template <>                                                                               \
    void CppProtobufSerializer::SetArray<TYPE>(const char *data, int size, pb::Message *msg,  \
                                               const pb::FieldDescriptor *field) const        \
    {                                                                                         \
      auto ref = msg->GetReflection();                                                        \
      auto array = reinterpret_cast<const TYPE *>(data);                                      \
      for (int i = 0; i < size; i++)                                                          \
      {                                                                                       \
        ref->Add##PB_NAME(msg, field, array[i]);                                              \
      }                                                                                       \
    }                                                                                         \
                                                                                              \
    template <>                                                                               \
    void CppProtobufSerializer::SetDynamicArray<TYPE>(const char *data, pb::Message *msg,     \
                                                      const pb::FieldDescriptor *field) const \
    {                                                                                         \
      auto ref = msg->GetReflection();                                                        \
      auto &vec = *reinterpret_cast<const std::vector<TYPE> *>(data);                         \
                                                                                              \
      for (auto value : vec)                                                                  \
      {                                                                                       \
        ref->Add##PB_NAME(msg, field, value);                                                 \
      }                                                                                       \
    }

    DEFINE_SET_METHODS(String, std::string)
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
    void CppProtobufSerializer::SetSingle<ros_message_t>(const char* data, const ts_introspection::MessageMembers* members,
                                                         pb::Message* msg, const pb::FieldDescriptor* field) const
    {
      auto sub_message = message_factory_.Create(members->message_name_);
      auto sub_desc = sub_message->GetDescriptor();
      FillMessage(data, members, sub_message, sub_desc);

      auto ref = msg->GetReflection();
      ref->SetAllocatedMessage(msg, sub_message, field);
    }

    template <>
    void CppProtobufSerializer::SetArray<ros_message_t>(const char* data, const ts_introspection::MessageMembers* members,
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
    void CppProtobufSerializer::SetDynamicArray<ros_message_t>(const char* data, const ts_introspection::MessageMembers* members,
                                                               pb::Message* msg, const pb::FieldDescriptor* field) const
    {
      auto vec = reinterpret_cast<const std::vector<char>*>(data);

      auto arr_data = vec->data();
      auto arr_size = vec->size() / members->size_of_;

      ThrowIfInvalidProtobufArraySize(arr_size);

      SetArray<ros_message_t>(arr_data, members, arr_size, msg, field);
    }

    template <typename T>
    void CppProtobufSerializer::Set(const char* data, const ts_introspection::MessageMember* member,
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
    void CppProtobufSerializer::Set<ros_message_t>(const char* data, const ts_introspection::MessageMember* member,
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

    void CppProtobufSerializer::FillMessage(const char* data, const ts_introspection::MessageMembers* members,
                                            pb::Message* msg, const pb::Descriptor* desc) const
    {
      for (uint32_t i = 0; i < members->member_count_; i++)
      {
        auto member = members->members_ + i;
        auto member_data = data + member->offset_;

        switch (member->type_id_)
        {
        case ts_introspection::ROS_TYPE_STRING:
          Set<std::string>(member_data, member, msg, desc);
          break;
        case ts_introspection::ROS_TYPE_BOOLEAN:
          Set<bool>(member_data, member, msg, desc);
          break;
        case ts_introspection::ROS_TYPE_BYTE:
          Set<uint8_t>(member_data, member, msg, desc); //-V1037
          break;
        case ts_introspection::ROS_TYPE_CHAR:
          Set<char>(member_data, member, msg, desc);
          break;
        case ts_introspection::ROS_TYPE_FLOAT:
          Set<float>(member_data, member, msg, desc);
          break;
        case ts_introspection::ROS_TYPE_DOUBLE:
          Set<double>(member_data, member, msg, desc);
          break;
        case ts_introspection::ROS_TYPE_INT8:
          Set<int8_t>(member_data, member, msg, desc);
          break;
        case ts_introspection::ROS_TYPE_INT16:
          Set<int16_t>(member_data, member, msg, desc);
          break;
        case ts_introspection::ROS_TYPE_INT32:
          Set<int32_t>(member_data, member, msg, desc);
          break;
        case ts_introspection::ROS_TYPE_INT64:
          Set<int64_t>(member_data, member, msg, desc);
          break;
        case ts_introspection::ROS_TYPE_UINT8:
          Set<uint8_t>(member_data, member, msg, desc);
          break;
        case ts_introspection::ROS_TYPE_UINT16:
          Set<uint16_t>(member_data, member, msg, desc);
          break;
        case ts_introspection::ROS_TYPE_UINT32:
          Set<uint32_t>(member_data, member, msg, desc);
          break;
        case ts_introspection::ROS_TYPE_UINT64:
          Set<uint64_t>(member_data, member, msg, desc);
          break;
        case ts_introspection::ROS_TYPE_MESSAGE:
          Set<ros_message_t>(member_data, member, msg, desc);
          break;
        case ts_introspection::ROS_TYPE_LONG_DOUBLE:
        case ts_introspection::ROS_TYPE_WSTRING:
        case ts_introspection::ROS_TYPE_WCHAR:
          throw std::logic_error("Wide character/string serialization is unsupported.");
        }
      }
    }

    const std::string CppProtobufSerializer::Serialize(const void* data)
    {
      std::unique_ptr<pb::Message> msg(message_factory_.Create());
      auto msg_desc = msg->GetDescriptor();
      FillMessage(static_cast<const char*>(data), members_, msg.get(), msg_desc);
      return msg->SerializeAsString();
    }

    const std::string CppProtobufSerializer::GetMessageStringDescriptor() const
    {
      return message_factory_.GetMessageStringDescriptor();
    }

  } // namespace rmw
} // namespace eCAL

#ifdef _MSC_VER
#pragma warning(pop)
#endif
