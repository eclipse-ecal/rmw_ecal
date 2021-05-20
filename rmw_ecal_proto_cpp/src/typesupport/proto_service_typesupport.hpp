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
#include <rosidl_typesupport_protobuf/service_type_support.hpp>

#include <rmw_ecal_shared_cpp/service_typesupport.hpp>

namespace eCAL
{
  namespace rmw
  {

    class ProtoServiceTypeSupport : public ServiceTypeSupport
    {
      const rosidl_typesupport_protobuf::service_type_support_t *type_support_;

      const rosidl_typesupport_protobuf::message_type_support_t *GetRequestMembers() const
      {
        return static_cast<const rosidl_typesupport_protobuf::message_type_support_t *>(type_support_->request->data);
      }

      const rosidl_typesupport_protobuf::message_type_support_t *GetResponseMembers() const
      {
        return static_cast<const rosidl_typesupport_protobuf::message_type_support_t *>(type_support_->response->data);
      }

    public:
      explicit ProtoServiceTypeSupport(const rosidl_typesupport_protobuf::service_type_support_t *type_support)
          : type_support_{type_support}
      {
      }

      virtual const std::string GetServiceNamespace() const override
      {
        return type_support_->service_namespace;
      }

      virtual const std::string GetServiceSimpleName() const override
      {
        return type_support_->service_name;
      }

      virtual const std::string GetServiceName() const override
      {
        return GetServiceNamespace() + "::" + GetServiceSimpleName();
      }

      virtual const std::string GetRequestMessageNamespace() const override
      {
        return GetRequestMembers()->message_namespace;
      }

      virtual const std::string GetRequestMessageName() const override
      {
        return GetRequestMembers()->message_name;
      }

      virtual const std::string GetResponseMessageNamespace() const override
      {
        return GetResponseMembers()->message_namespace;
      }

      virtual const std::string GetResponseMessageName() const override
      {
        return GetResponseMembers()->message_name;
      }

      virtual const std::string SerializeRequest(const void *data) override
      {
        std::string serialized_data{};
        GetRequestMembers()->serialize(data, serialized_data);
        return serialized_data;
      }

      virtual const std::string SerializeResponse(const void *data) override
      {
        std::string serialized_data{};
        GetResponseMembers()->serialize(data, serialized_data);
        return serialized_data;
      }

      virtual void DeserializeRequest(void *message, const void *serialized_data, size_t size) override
      {
        GetRequestMembers()->deserialize(message, serialized_data, size);
      }

      virtual void DeserializeResponse(void *message, const void *serialized_data, size_t size) override
      {
        GetResponseMembers()->deserialize(message, serialized_data, size);
      }
    };

  } // namespace rmw
} // namespace eCAL