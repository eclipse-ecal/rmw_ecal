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

#pragma once

#include <string>
#include <limits>
#include <stdexcept>

#include <google/protobuf/descriptor.pb.h>

namespace eCAL
{
namespace rmw
{

inline void ThrowIfInvalidProtobufArraySize(size_t size)
{
	int max_int = std::numeric_limits<int>::max();
	if (size > static_cast<size_t>(max_int))
		throw std::logic_error("Maximum array size when using protobuf serialization is: " + std::to_string(max_int));
}

inline google::protobuf::FieldDescriptorProto_Type GetProtobufType(const rosidl_typesupport_introspection_cpp::MessageMember *const member)
{
	using pb_field_type = google::protobuf::FieldDescriptorProto_Type;
	switch (member->type_id_)
	{
	case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_STRING:
		return pb_field_type::FieldDescriptorProto_Type_TYPE_STRING;
	case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_BOOLEAN:
		return pb_field_type::FieldDescriptorProto_Type_TYPE_BOOL;
	case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_CHAR:
	case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT8:
	case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT16:
	case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT32:
		return pb_field_type::FieldDescriptorProto_Type_TYPE_INT32;
	case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_FLOAT:
		return pb_field_type::FieldDescriptorProto_Type_TYPE_FLOAT;
		break;
	case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE:
		return pb_field_type::FieldDescriptorProto_Type_TYPE_DOUBLE;
		break;
	case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT64:
		return pb_field_type::FieldDescriptorProto_Type_TYPE_INT64;
	case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_BYTE:
	case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT8:
	case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT16:
	case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT32:
		return pb_field_type::FieldDescriptorProto_Type_TYPE_UINT32;
	case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT64:
		return pb_field_type::FieldDescriptorProto_Type_TYPE_UINT64;
	case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE:
		return pb_field_type::FieldDescriptorProto_Type_TYPE_MESSAGE;
	case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_LONG_DOUBLE:
	case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_WSTRING:
	case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_WCHAR:
	default:
		throw std::logic_error("Unsupported type.");
	}
}

inline google::protobuf::FieldDescriptorProto_Type GetProtobufType(const rosidl_typesupport_introspection_c__MessageMember *const member)
{
	using pb_field_type = google::protobuf::FieldDescriptorProto_Type;
	switch (member->type_id_)
	{
	case rosidl_typesupport_introspection_c__ROS_TYPE_STRING:
		return pb_field_type::FieldDescriptorProto_Type_TYPE_STRING;
	case rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN:
		return pb_field_type::FieldDescriptorProto_Type_TYPE_BOOL;
	case rosidl_typesupport_introspection_c__ROS_TYPE_CHAR:
	case rosidl_typesupport_introspection_c__ROS_TYPE_INT8:
	case rosidl_typesupport_introspection_c__ROS_TYPE_INT16:
	case rosidl_typesupport_introspection_c__ROS_TYPE_INT32:
		return pb_field_type::FieldDescriptorProto_Type_TYPE_INT32;
	case rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT:
		return pb_field_type::FieldDescriptorProto_Type_TYPE_FLOAT;
		break;
	case rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE:
		return pb_field_type::FieldDescriptorProto_Type_TYPE_DOUBLE;
		break;
	case rosidl_typesupport_introspection_c__ROS_TYPE_INT64:
		return pb_field_type::FieldDescriptorProto_Type_TYPE_INT64;
	case rosidl_typesupport_introspection_c__ROS_TYPE_BYTE:
	case rosidl_typesupport_introspection_c__ROS_TYPE_UINT8:
	case rosidl_typesupport_introspection_c__ROS_TYPE_UINT16:
	case rosidl_typesupport_introspection_c__ROS_TYPE_UINT32:
		return pb_field_type::FieldDescriptorProto_Type_TYPE_UINT32;
	case rosidl_typesupport_introspection_c__ROS_TYPE_UINT64:
		return pb_field_type::FieldDescriptorProto_Type_TYPE_UINT64;
	case rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE:
		return pb_field_type::FieldDescriptorProto_Type_TYPE_MESSAGE;
	case rosidl_typesupport_introspection_c__ROS_TYPE_LONG_DOUBLE:
	case rosidl_typesupport_introspection_c__ROS_TYPE_WSTRING:
	case rosidl_typesupport_introspection_c__ROS_TYPE_WCHAR:
	default:
		throw std::logic_error("Unsupported type.");
	}
}
} // namespace rmw
} // namespace eCAL