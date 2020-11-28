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

#include "deserializer_cpp.hpp"

#include <rosidl_typesupport_introspection_cpp/message_introspection.hpp>

#include "internal/serialization/deserializer.hpp"
#include "internal/serialization/protobuf/message_factory.hpp"

namespace eCAL
{
namespace rmw
{

class CppProtobufDeserializer : public Deserializer
{
	const rosidl_typesupport_introspection_cpp::MessageMembers *members_;
	const ProtobufMessageFactory message_factory_;

	template <typename T>
	void SetSingle(char *ros_member,
				   const google::protobuf::Message *msg,
				   const google::protobuf::FieldDescriptor *field) const;

	template <typename T>
	void SetSingle(char *ros_member,
				   const rosidl_typesupport_introspection_cpp::MessageMembers *members,
				   const google::protobuf::Message *msg,
				   const google::protobuf::FieldDescriptor *field) const;

	template <typename T>
	void SetArray(char *ros_member,
				  int size,
				  const google::protobuf::Message *msg,
				  const google::protobuf::FieldDescriptor *field) const;

	template <typename T>
	void SetArray(char *ros_member,
				  const rosidl_typesupport_introspection_cpp::MessageMembers *members,
				  int size,
				  const google::protobuf::Message *msg,
				  const google::protobuf::FieldDescriptor *field) const;

	template <typename T>
	void SetDynamicArray(char *ros_member,
						 const google::protobuf::Message *msg,
						 const google::protobuf::FieldDescriptor *field) const;

	template <typename T>
	void SetDynamicArray(char *ros_member,
						 const rosidl_typesupport_introspection_cpp::MessageMembers *members,
						 const google::protobuf::Message *msg,
						 const google::protobuf::FieldDescriptor *field) const;

	template <typename T>
	void Set(char *ros_member,
			 const rosidl_typesupport_introspection_cpp::MessageMember *member,
			 const google::protobuf::Message *msg,
			 const google::protobuf::FieldDescriptor *field) const;

	void FillMessage(char *ros_msg,
					 const rosidl_typesupport_introspection_cpp::MessageMembers *members,
					 const google::protobuf::Message *msg) const;

public:
	explicit CppProtobufDeserializer(const rosidl_typesupport_introspection_cpp::MessageMembers *members)
		: members_(members), message_factory_(members)
	{
	}

	virtual void Deserialize(void *message, const void *serialized_data, size_t size) override;
};

} // namespace rmw
} // namespace eCAL