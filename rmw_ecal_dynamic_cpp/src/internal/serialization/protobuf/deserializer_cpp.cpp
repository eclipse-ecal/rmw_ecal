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

#include "deserializer_cpp.hpp"

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

    #define DEFINE_SET_METHODS(PB_NAME, TYPE)                                                   \
    template <>                                                                                 \
    void CppProtobufDeserializer::SetSingle<TYPE>(char *ros_member,                             \
                                                  const pb::Message *msg,                       \
                                                  const pb::FieldDescriptor *field) const       \
    {                                                                                           \
      auto data = reinterpret_cast<TYPE *>(ros_member);                                         \
      auto ref = msg->GetReflection();                                                          \
      *data = ref->Get##PB_NAME(*msg, field);                                                   \
    }                                                                                           \
                                                                                                \
    template <>                                                                                 \
    void CppProtobufDeserializer::SetDynamicArray<TYPE>(char *ros_member,                       \
                                                        const pb::Message *msg,                 \
                                                        const pb::FieldDescriptor *field) const \
    {                                                                                           \
                                                                                                \
      auto data = reinterpret_cast<std::vector<TYPE> *>(ros_member);                            \
      auto ref = msg->GetReflection();                                                          \
      auto size = ref->FieldSize(*msg, field);                                                  \
      data->reserve(size);                                                                      \
      for (int i = 0; i < size; i++)                                                            \
      {                                                                                         \
        data->push_back(ref->GetRepeated##PB_NAME(*msg, field, i));                             \
      }                                                                                         \
    }                                                                                           \
                                                                                                \
    template <>                                                                                 \
    void CppProtobufDeserializer::SetArray<TYPE>(char *ros_member,                              \
                                                 int size,                                      \
                                                 const pb::Message *msg,                        \
                                                 const pb::FieldDescriptor *field) const        \
    {                                                                                           \
      auto data = reinterpret_cast<TYPE *>(ros_member);                                         \
      auto ref = msg->GetReflection();                                                          \
      for (int i = 0; i < size; i++, data++)                                                    \
      {                                                                                         \
        *data = ref->GetRepeated##PB_NAME(*msg, field, i);                                      \
      }                                                                                         \
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
    void CppProtobufDeserializer::SetSingle<std::string>(char* ros_member,
                                                         const pb::Message* msg,
                                                         const pb::FieldDescriptor* field) const
    {
      auto data = reinterpret_cast<std::string*>(ros_member);
      auto ref = msg->GetReflection();
      *data = ref->GetStringReference(*msg, field, data);
    }

    template <>
    void CppProtobufDeserializer::SetArray<std::string>(char* ros_member,
                                                        int size,
                                                        const pb::Message* msg,
                                                        const pb::FieldDescriptor* field) const
    {
      auto data = reinterpret_cast<std::string*>(ros_member);
      auto ref = msg->GetReflection();
      for (int i = 0; i < size; i++, data++)
      {
        *data = ref->GetRepeatedStringReference(*msg, field, i, data);
      }
    }

    template <>
    void CppProtobufDeserializer::SetDynamicArray<std::string>(char* ros_member,
                                                               const pb::Message* msg,
                                                               const pb::FieldDescriptor* field) const
    {
      auto data = reinterpret_cast<std::vector<std::string>*>(ros_member);
      auto ref = msg->GetReflection();
      auto size = ref->FieldSize(*msg, field);
      data->reserve(size);
      for (int i = 0; i < size; i++)
      {
        auto& str = ref->GetRepeatedStringReference(*msg, field, i, nullptr);
        data->push_back(str);
      }
    }

    template <>
    void CppProtobufDeserializer::SetSingle<ros_message_t>(char* ros_member,
                                                           const ts_introspection::MessageMembers* members,
                                                           const pb::Message* msg,
                                                           const pb::FieldDescriptor* field) const
    {
      auto ref = msg->GetReflection();
      auto& sub_message = ref->GetMessage(*msg, field);
      FillMessage(ros_member, members, &sub_message);
    }

    template <>
    void CppProtobufDeserializer::SetArray<ros_message_t>(char* ros_member,
                                                          const ts_introspection::MessageMembers* members,
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
    void CppProtobufDeserializer::SetDynamicArray<ros_message_t>(char* ros_member,
                                                                 const ts_introspection::MessageMembers* members,
                                                                 const pb::Message* msg,
                                                                 const pb::FieldDescriptor* field) const
    {
      auto data = reinterpret_cast<std::vector<char>*>(ros_member);
      auto ref = msg->GetReflection();
      auto size = ref->FieldSize(*msg, field);
      data->resize(size * members->size_of_);
      SetArray<ros_message_t>(data->data(), members, size, msg, field);
    }

    template <typename T>
    void CppProtobufDeserializer::Set(char* ros_member,
                                      const ts_introspection::MessageMember* member,
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
    void CppProtobufDeserializer::Set<ros_message_t>(char* ros_member,
                                                     const ts_introspection::MessageMember* member,
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

#undef DEFINE_SET_METHODS

    void CppProtobufDeserializer::FillMessage(char* ros_msg,
                                              const ts_introspection::MessageMembers* members,
                                              const pb::Message* msg) const
    {
      auto msg_desc = msg->GetDescriptor();
      for (uint32_t i = 0; i < members->member_count_; i++)
      {
        auto member = members->members_ + i;
        auto member_data = ros_msg + member->offset_;

        auto field = msg_desc->FindFieldByName(member->name_);

        switch (member->type_id_)
        {
        case ts_introspection::ROS_TYPE_STRING:
          Set<std::string>(member_data, member, msg, field);
          break;
        case ts_introspection::ROS_TYPE_BOOLEAN:
          Set<bool>(member_data, member, msg, field);
          break;
        case ts_introspection::ROS_TYPE_BYTE:
          Set<uint8_t>(member_data, member, msg, field); //-V1037
          break;
        case ts_introspection::ROS_TYPE_CHAR:
          Set<char>(member_data, member, msg, field);
          break;
        case ts_introspection::ROS_TYPE_FLOAT:
          Set<float>(member_data, member, msg, field);
          break;
        case ts_introspection::ROS_TYPE_DOUBLE:
          Set<double>(member_data, member, msg, field);
          break;
        case ts_introspection::ROS_TYPE_INT8:
          Set<int8_t>(member_data, member, msg, field);
          break;
        case ts_introspection::ROS_TYPE_INT16:
          Set<int16_t>(member_data, member, msg, field);
          break;
        case ts_introspection::ROS_TYPE_INT32:
          Set<int32_t>(member_data, member, msg, field);
          break;
        case ts_introspection::ROS_TYPE_INT64:
          Set<int64_t>(member_data, member, msg, field);
          break;
        case ts_introspection::ROS_TYPE_UINT8:
          Set<uint8_t>(member_data, member, msg, field);
          break;
        case ts_introspection::ROS_TYPE_UINT16:
          Set<uint16_t>(member_data, member, msg, field);
          break;
        case ts_introspection::ROS_TYPE_UINT32:
          Set<uint32_t>(member_data, member, msg, field);
          break;
        case ts_introspection::ROS_TYPE_UINT64:
          Set<uint64_t>(member_data, member, msg, field);
          break;
        case ts_introspection::ROS_TYPE_MESSAGE:
          Set<ros_message_t>(member_data, member, msg, field);
          break;
        case ts_introspection::ROS_TYPE_WSTRING:
        case ts_introspection::ROS_TYPE_WCHAR:
          throw std::logic_error("Wide character/string deserialization is unsupported.");
        }
      }
    }

    void CppProtobufDeserializer::Deserialize(void* message, const void* serialized_data, size_t size)
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
