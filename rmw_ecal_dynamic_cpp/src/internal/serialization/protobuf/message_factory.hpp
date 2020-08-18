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
#include <unordered_set>

#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>

#include <rosidl_typesupport_introspection_cpp/message_introspection.hpp>
#include <rosidl_typesupport_introspection_cpp/field_types.hpp>
#include <rosidl_typesupport_introspection_c/message_introspection.h>
#include <rosidl_typesupport_introspection_c/field_types.h>

#include "internal/common.hpp"
#include "internal/serialization/protobuf/helpers.hpp"

namespace eCAL
{
namespace rmw
{

class ProtobufMessageFactory
{
	using pb_label = google::protobuf::FieldDescriptorProto_Label;

	google::protobuf::DescriptorPool file_descriptor_pool_;
	mutable google::protobuf::DynamicMessageFactory prototype_factory_;

	const google::protobuf::FileDescriptor *file_descriptor_;
	const google::protobuf::Message *message_prototype_;

	std::string message_string_descriptor_;

	void RegisterType(google::protobuf::FileDescriptorProto *file,
					  google::protobuf::DescriptorProto *msg,
					  const rosidl_typesupport_introspection_cpp::MessageMembers *members,
					  std::unordered_set<std::string> &registered_types)
	{
		
		msg->set_name(members->message_name_);

		for (uint32_t field_index = 0; field_index < members->member_count_; field_index++)
		{
			auto member = members->members_ + field_index;

			auto field = msg->add_field();
			field->set_name(member->name_);
			field->set_type(GetProtobufType(member));
			field->set_number(field_index + 1);
			if (member->is_array_)
			{
				field->set_label(pb_label::FieldDescriptorProto_Label_LABEL_REPEATED);
			}
			else
			{
				field->set_label(pb_label::FieldDescriptorProto_Label_LABEL_REQUIRED);
			}

			if (member->type_id_ == rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE)
			{
				auto sub_members = GetMembers(member);
				field->set_type_name(sub_members->message_name_);

				if (!registered_types.count(sub_members->message_name_))
				{
					auto sub_msg = file->add_message_type();
					RegisterType(file, sub_msg, sub_members, registered_types);
					registered_types.insert(sub_members->message_name_);
				}
			}
		}
	}

	void RegisterType(google::protobuf::FileDescriptorProto *file,
					  google::protobuf::DescriptorProto *msg,
					  const rosidl_typesupport_introspection_c__MessageMembers *members,
					  std::unordered_set<std::string> &registered_types)
	{

		msg->set_name(members->message_name_);

		for (uint32_t field_index = 0; field_index < members->member_count_; field_index++)
		{
			auto member = members->members_ + field_index;

			auto field = msg->add_field();
			field->set_name(member->name_);
			field->set_type(GetProtobufType(member));
			field->set_number(field_index + 1);
			if (member->is_array_)
			{
				field->set_label(pb_label::FieldDescriptorProto_Label_LABEL_REPEATED);
			}
			else
			{
				field->set_label(pb_label::FieldDescriptorProto_Label_LABEL_REQUIRED);
			}

			if (member->type_id_ == rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE)
			{
				auto sub_members = GetMembers(member);
				field->set_type_name(sub_members->message_name_);

				if (!registered_types.count(sub_members->message_name_))
				{
					auto sub_msg = file->add_message_type();
					RegisterType(file, sub_msg, sub_members, registered_types);
					registered_types.insert(sub_members->message_name_);
				}
			}
		}
	}

public:
	explicit ProtobufMessageFactory(const rosidl_typesupport_introspection_cpp::MessageMembers *members)
	{
		google::protobuf::FileDescriptorSet file_desc_set;

		auto file_desc_proto = file_desc_set.add_file();
		file_desc_proto->set_package("pb.ros2");
		file_desc_proto->set_name(members->message_name_ + std::string(".proto"));

		auto msg = file_desc_proto->add_message_type();
		std::unordered_set<std::string> registered_types;
		RegisterType(file_desc_proto, msg, members, registered_types);

		file_descriptor_ = file_descriptor_pool_.BuildFile(file_desc_set.file(0));
		message_prototype_ = prototype_factory_.GetPrototype(file_descriptor_->FindMessageTypeByName(members->message_name_));
		message_string_descriptor_ = file_desc_set.SerializeAsString();
	}

	explicit ProtobufMessageFactory(const rosidl_typesupport_introspection_c__MessageMembers *members)
	{
		google::protobuf::FileDescriptorSet file_desc_set;

		auto file_desc_proto = file_desc_set.add_file();
		file_desc_proto->set_package("pb.ros2");
		file_desc_proto->set_name(members->message_name_ + std::string(".proto"));

		auto msg = file_desc_proto->add_message_type();
		std::unordered_set<std::string> registered_types;
		RegisterType(file_desc_proto, msg, members, registered_types);

		file_descriptor_ = file_descriptor_pool_.BuildFile(file_desc_set.file(0));
		message_prototype_ = prototype_factory_.GetPrototype(file_descriptor_->FindMessageTypeByName(members->message_name_));
		message_string_descriptor_ = file_desc_set.SerializeAsString();
	}

	google::protobuf::Message *Create() const
	{
		return message_prototype_->New();
	}

	google::protobuf::Message *Create(const std::string &name) const
	{
		auto msg_desc = file_descriptor_->FindMessageTypeByName(name);
		return prototype_factory_.GetPrototype(msg_desc)->New();
	}

	const std::string &GetMessageStringDescriptor() const
	{
		return message_string_descriptor_;
	}
};

} // namespace rmw
} // namespace eCAL