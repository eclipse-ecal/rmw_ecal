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

#include <stdexcept>

#include <rosidl_typesupport_protobuf/service_type_support.hpp>
#include <rosidl_typesupport_protobuf/message_type_support.hpp>
#include <rosidl_typesupport_protobuf/service_type_support.hpp>
#include <rosidl_typesupport_protobuf_c/identifier.hpp>
#include <rosidl_typesupport_protobuf_cpp/identifier.hpp>

inline const rosidl_typesupport_protobuf::message_type_support_t *GetTypeSupport(const rosidl_message_type_support_t *type_support)
{
  auto ts{get_message_typesupport_handle(type_support, rosidl_typesupport_protobuf_cpp::identifier)};
  if (ts != nullptr)
  {
    return static_cast<const rosidl_typesupport_protobuf::message_type_support_t *>(ts->data);
  }

  ts = get_message_typesupport_handle(type_support, rosidl_typesupport_protobuf_c::identifier);
  if (ts != nullptr)
  {
    return static_cast<const rosidl_typesupport_protobuf::message_type_support_t *>(ts->data);
  }
  throw std::runtime_error{"Unsupported type support."};
}

inline const rosidl_typesupport_protobuf::service_type_support_t *GetTypeSupport(const rosidl_service_type_support_t *type_support)
{
  auto ts{get_service_typesupport_handle(type_support, rosidl_typesupport_protobuf_cpp::identifier)};
  if (ts != nullptr)
  {
    return static_cast<const rosidl_typesupport_protobuf::service_type_support_t *>(ts->data);
  }

  ts = get_service_typesupport_handle(type_support, rosidl_typesupport_protobuf_c::identifier);
  if (ts != nullptr)
  {
    return static_cast<const rosidl_typesupport_protobuf::service_type_support_t *>(ts->data);
  }
  throw std::runtime_error{"Unsupported type support."};
}
