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

#pragma once

#include <string>
#include <memory>

#include <rosidl_typesupport_protobuf/message_type_support.hpp>

#include <rmw_ecal_shared_cpp/string_functions.hpp>
#include <rmw_ecal_shared_cpp/message_typesupport.hpp>

namespace eCAL
{
  namespace rmw
  {

    class ProtoMessageTypeSupport : public MessageTypeSupport
    {
      const rosidl_typesupport_protobuf::message_type_support_t *type_support_;

    public:
      explicit ProtoMessageTypeSupport(const rosidl_typesupport_protobuf::message_type_support_t *type_support)
          : type_support_{type_support}
      {
      }

      virtual const std::string GetMessageNamespace() const override
      {
        return ReplaceString(ReplaceString(type_support_->message_namespace, "::", "/"), "__", "/");
      }

      virtual const std::string GetMessageSimpleName() const override
      {
        return type_support_->message_name ? type_support_->message_name : "";
      }

      virtual const std::string GetMessageName() const override
      {
        return "proto:" + GetMessageSimpleName();
      }

      virtual size_t GetTypeSize() const override
      {
        return 0;
      }

      virtual const std::string Serialize(const void *data) override
      {
        std::string serialized_data{};
        type_support_->serialize(data, serialized_data);
        return serialized_data;
      }

      virtual void Deserialize(void *message, const void *serialized_data, size_t size) override
      {
        type_support_->deserialize(message, serialized_data, size);
      }

      virtual std::string GetTypeDescriptor() const override
      {
        return type_support_->get_descriptor();
      }
    };

  } // namespace rmw
} // namespace eCAL