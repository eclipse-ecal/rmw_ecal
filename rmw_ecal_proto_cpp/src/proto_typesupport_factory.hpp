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

#include <rmw_ecal_shared_cpp/typesupport_factory.hpp>

#include "typesupport/proto_message_typesupport.hpp"
#include "typesupport/proto_service_typesupport.hpp"

#include "common.hpp"

namespace eCAL
{
  namespace rmw
  {
    class ProtoTypeSupportFactory : public TypesupportFactory
    {
      MessageTypeSupport *Create(const rosidl_message_type_support_t *type_support) const override
      {
        return new ProtoMessageTypeSupport{GetTypeSupport(type_support)};
      }

      ServiceTypeSupport *Create(const rosidl_service_type_support_t *type_support) const override
      {
        return new ProtoServiceTypeSupport{GetTypeSupport(type_support)};
      }
    };

  } // namespace rmw
} // namespace eCAL