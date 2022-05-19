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

#include <rosidl_typesupport_protobuf/message_type_support.hpp>

#include <rmw_ecal_shared_cpp/serializer.hpp>

namespace eCAL
{
  namespace rmw
  {

    class ProtoSerializer : public Serializer
    {
      const rosidl_typesupport_protobuf::message_type_support_t *typesupport_;

    public:
      explicit ProtoSerializer(const rosidl_typesupport_protobuf::message_type_support_t *typesupport)
          : typesupport_{typesupport}
      {
      }

      virtual const std::string Serialize(const void *data) override
      {
        std::string serialized_message{};
        typesupport_->serialize(data, serialized_message);
        return serialized_message;
      }

      virtual const std::string GetMessageStringDescriptor() const override
      {
        return typesupport_->get_descriptor();
      }
    };

  } // namespace rmw
} // namespace eCAL
