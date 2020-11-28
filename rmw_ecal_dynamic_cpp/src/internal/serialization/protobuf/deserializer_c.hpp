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

#include <rosidl_typesupport_introspection_c/message_introspection.h>

#include "internal/serialization/deserializer.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4127)
#endif
#include "internal/serialization/protobuf/message_factory.hpp"
#ifdef _MSC_VER
#pragma warning(pop)
#endif

namespace eCAL
{
namespace rmw
{

class CProtobufDeserializer : public Deserializer
{
	const rosidl_typesupport_introspection_c__MessageMembers *members_;
	const ProtobufMessageFactory message_factory_;

	template <typename T>
	void SetSingle(char *ros_member,
				   const google::protobuf::Message *msg,
				   const google::protobuf::FieldDescriptor *field) const;

	template <typename T>
	void SetSingle(char *ros_member,
				   const rosidl_typesupport_introspection_c__MessageMembers *members,
				   const google::protobuf::Message *msg,
				   const google::protobuf::FieldDescriptor *field) const;

	template <typename T>
	void SetArray(char *ros_member,
				  int array_size,
				  const google::protobuf::Message *msg,
				  const google::protobuf::FieldDescriptor *field) const;

	template <typename T>
	void SetArray(char *ros_member,
				  const rosidl_typesupport_introspection_c__MessageMembers *members,
				  int size,
				  const google::protobuf::Message *msg,
				  const google::protobuf::FieldDescriptor *field) const;

	template <typename T>
	void SetDynamicArray(char *ros_member,
						 const google::protobuf::Message *msg,
						 const google::protobuf::FieldDescriptor *field) const;

	template <typename T>
	void SetDynamicArray(char *ros_member,
						 const rosidl_typesupport_introspection_c__MessageMembers *members,
						 const google::protobuf::Message *msg,
						 const google::protobuf::FieldDescriptor *field) const;

	template <typename T>
	void Set(char *ros_member,
			 const rosidl_typesupport_introspection_c__MessageMember *member,
			 const google::protobuf::Message *msg,
			 const google::protobuf::FieldDescriptor *field) const;

	void FillMessage(char *ros_msg,
					 const rosidl_typesupport_introspection_c__MessageMembers *members,
					 const google::protobuf::Message *msg) const;

public:
	explicit CProtobufDeserializer(const rosidl_typesupport_introspection_c__MessageMembers *members)
		: members_(members), message_factory_(members)
	{
	}

	virtual void Deserialize(void *message, const void *serialized_data, size_t size) override;
};

} // namespace rmw
} // namespace eCAL