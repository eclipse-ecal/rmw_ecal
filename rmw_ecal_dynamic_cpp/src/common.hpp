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

#include <rosidl_typesupport_introspection_cpp/message_introspection.hpp>
#include <rosidl_typesupport_introspection_c/message_introspection.h>
#include <rosidl_typesupport_introspection_cpp/service_introspection.hpp>
#include <rosidl_typesupport_introspection_c/service_introspection.h>

#include <rmw_ecal_shared_cpp/rosidl_generator_c_pkg_adapter.hpp>

using array_size_t = uint64_t;
//Represents arbitrary ros message type, used for consistency
//of serialization/deserialization template functions.
struct ros_message_t;

inline const rosidl_typesupport_introspection_cpp::MessageMembers *GetMembers(
    const rosidl_typesupport_introspection_cpp::MessageMember *member)
{
  return static_cast<const rosidl_typesupport_introspection_cpp::MessageMembers *>(member->members_->data);
}

inline const rosidl_typesupport_introspection_c__MessageMembers *GetMembers(
    const rosidl_typesupport_introspection_c__MessageMember *member)
{
  return static_cast<const rosidl_typesupport_introspection_c__MessageMembers *>(member->members_->data);
}

inline const rosidl_typesupport_introspection_cpp::MessageMembers *GetCppMembers(
    const rosidl_message_type_support_t *type_support)
{
  return static_cast<const rosidl_typesupport_introspection_cpp::MessageMembers *>(type_support->data);
}

inline const rosidl_typesupport_introspection_c__MessageMembers *GetCMembers(
    const rosidl_message_type_support_t *type_support)
{
  return static_cast<const rosidl_typesupport_introspection_c__MessageMembers *>(type_support->data);
}

inline const rosidl_typesupport_introspection_cpp::ServiceMembers *GetCppMembers(
    const rosidl_service_type_support_t *type_support)
{
  return static_cast<const rosidl_typesupport_introspection_cpp::ServiceMembers *>(type_support->data);
}

inline const rosidl_typesupport_introspection_c__ServiceMembers *GetCMembers(
    const rosidl_service_type_support_t *type_support)
{
  return static_cast<const rosidl_typesupport_introspection_c__ServiceMembers *>(type_support->data);
}