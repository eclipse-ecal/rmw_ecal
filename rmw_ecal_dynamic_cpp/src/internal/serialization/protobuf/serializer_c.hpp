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

#include <rosidl_typesupport_introspection_c/message_introspection.h>

#include "internal/serialization/serializer.hpp"
#include "internal/serialization/protobuf/message_factory.hpp"

namespace eCAL
{
namespace rmw
{

class CProtobufSerializer : public Serializer
{
	const rosidl_typesupport_introspection_c__MessageMembers *members_;
	const ProtobufMessageFactory message_factory_;

	template <typename T>
	void SetSingle(const char *data, google::protobuf::Message *msg, const google::protobuf::FieldDescriptor *field) const;

	template <typename T>
	void SetSingle(const char *data, const rosidl_typesupport_introspection_c__MessageMembers *members,
				   google::protobuf::Message *msg, const google::protobuf::FieldDescriptor *field) const;

	template <typename T>
	void SetArray(const char *data, int size, google::protobuf::Message *msg,
				  const google::protobuf::FieldDescriptor *field) const;

	template <typename T>
	void SetArray(const char *data, const rosidl_typesupport_introspection_c__MessageMembers *members,
				  int size, google::protobuf::Message *msg, const google::protobuf::FieldDescriptor *field) const;

	template <typename T>
	void SetDynamicArray(const char *data, google::protobuf::Message *msg,
						 const google::protobuf::FieldDescriptor *field) const;

	template <typename T>
	void SetDynamicArray(const char *data, const rosidl_typesupport_introspection_c__MessageMembers *members,
						 google::protobuf::Message *msg, const google::protobuf::FieldDescriptor *field) const;

	template <typename T>
	void Set(const char *data, const rosidl_typesupport_introspection_c__MessageMember *member,
			 google::protobuf::Message *msg, const google::protobuf::Descriptor *desc) const;

	void FillMessage(const char *data, const rosidl_typesupport_introspection_c__MessageMembers *members,
					 google::protobuf::Message *msg, const google::protobuf::Descriptor *desc) const;

public:
	explicit CProtobufSerializer(const rosidl_typesupport_introspection_c__MessageMembers *members)
		: members_(members), message_factory_(members)
	{
	}

	virtual const std::string Serialize(const void *data) override;
	virtual const std::string GetMessageStringDescriptor() const override;
};

} // namespace rmw
} // namespace eCAL