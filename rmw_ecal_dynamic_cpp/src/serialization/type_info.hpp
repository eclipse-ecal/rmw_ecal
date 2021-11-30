
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

#include <unordered_set>
#include <iostream>

#include <rosidl_typesupport_introspection_cpp/message_introspection.hpp>
#include <rosidl_typesupport_introspection_cpp/field_types.hpp>
#include <rosidl_typesupport_introspection_c/message_introspection.h>
#include <rosidl_typesupport_introspection_c/field_types.h>

#include "common.hpp"

namespace eCAL
{
  namespace rmw
  {
    namespace TypeInfo
    {

    namespace ts_cpp = rosidl_typesupport_introspection_cpp;

    static std::unordered_set<const char*> analyzed_types_;
    static std::unordered_set<const char*> memcopyable_types_;

    template<typename ts_introspection>
    inline void SetAnalyzed(const ts_introspection *members)
    {
      auto type_id = members->message_name_;
      analyzed_types_.emplace(type_id);
    }

    template<typename ts_introspection>
    inline bool AlreadyAnalyzed(const ts_introspection *members)
    {
      auto type_id = members->message_name_;
      return analyzed_types_.find(type_id) != analyzed_types_.end();
    }

    template<typename ts_introspection>
    inline void SetMemcopyable(const ts_introspection *members)
    {
      auto type_id = members->message_name_;
      memcopyable_types_.emplace(type_id);
    }

    template<typename ts_introspection>
    inline bool IsMemcopyable(const ts_introspection *members)
    {
      auto type_id = members->message_name_;
      return memcopyable_types_.find(type_id) != memcopyable_types_.end();
    }

    inline void AnalyzeType(const ts_cpp::MessageMembers *members)
    {
      if(AlreadyAnalyzed(members)) return;

      bool memcpyable = true;
      auto struct_size = members->size_of_;
      size_t actual_data_size = 0;

      for (uint32_t i = 0; i < members->member_count_; i++)
      {
        const auto member = members->members_ + i;

	//Detect if there was padding before this member
	if(member->offset_ != actual_data_size)
	{
	  memcpyable = false;
	}
        if (member->is_array_)
        {
          if (!(member->array_size_ > 0 && !member->is_upper_bound_))
          {
            memcpyable = false;
          }
        }
        switch (member->type_id_)
        {
        case ts_cpp::ROS_TYPE_STRING:
	  memcpyable = false;
          return;
        case ts_cpp::ROS_TYPE_BOOLEAN:
          actual_data_size += sizeof(bool);
          break;
        case ts_cpp::ROS_TYPE_BYTE:
          actual_data_size += sizeof(uint8_t);
          break;
        case ts_cpp::ROS_TYPE_CHAR:
          actual_data_size += sizeof(char);
          break;
        case ts_cpp::ROS_TYPE_FLOAT:
          actual_data_size += sizeof(float);
          break;
        case ts_cpp::ROS_TYPE_DOUBLE:
          actual_data_size += sizeof(double);
          break;
        case ts_cpp::ROS_TYPE_LONG_DOUBLE:
          actual_data_size += sizeof(long double);
          break;
        case ts_cpp::ROS_TYPE_INT8:
          actual_data_size += sizeof(int8_t);
          break;
        case ts_cpp::ROS_TYPE_INT16:
          actual_data_size += sizeof(int16_t);
          break;
        case ts_cpp::ROS_TYPE_INT32:
          actual_data_size += sizeof(int32_t);
          break;
        case ts_cpp::ROS_TYPE_INT64:
          actual_data_size += sizeof(int64_t);
          break;
        case ts_cpp::ROS_TYPE_UINT8:
          actual_data_size += sizeof(uint8_t);
          break;
        case ts_cpp::ROS_TYPE_UINT16:
          actual_data_size += sizeof(uint16_t);
          break;
        case ts_cpp::ROS_TYPE_UINT32:
          actual_data_size += sizeof(uint32_t);
          break;
        case ts_cpp::ROS_TYPE_UINT64:
          actual_data_size += sizeof(uint64_t);
	  break;
        case ts_cpp::ROS_TYPE_MESSAGE:
	  {
            auto msg_members = GetMembers(member);
	    AnalyzeType(msg_members);
	    if(IsMemcopyable(msg_members))
	    {
              actual_data_size += msg_members->size_of_;
	    }
	    else
	    {
              memcpyable = false;
	    }
	  }
          break;
          //not documented
        case ts_cpp::ROS_TYPE_WSTRING:
        case ts_cpp::ROS_TYPE_WCHAR:
          throw std::logic_error("Wide character/string serialization is unsupported.");
	}
      }

      SetAnalyzed(members);
      if(actual_data_size == struct_size && memcpyable)
      {
	SetMemcopyable(members);
      }
    }

    inline void AnalyzeType(const rosidl_typesupport_introspection_c__MessageMembers *members)
    {
      if(AlreadyAnalyzed(members)) return;

      bool memcpyable = true;
      auto struct_size = members->size_of_;
      size_t actual_data_size = 0;

      for (uint32_t i = 0; i < members->member_count_; i++)
      {
        const auto member = members->members_ + i;

	//Detect if there was padding before this member
	if(member->offset_ != actual_data_size)
	{
	  memcpyable = false;
	}
        if (member->is_array_)
        {
          if (!(member->array_size_ > 0 && !member->is_upper_bound_))
          {
            memcpyable = false;
          }
        }
        switch (member->type_id_)
        {
        case rosidl_typesupport_introspection_c__ROS_TYPE_STRING:
	  memcpyable = false;
          return;
        case rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN:
          actual_data_size += sizeof(bool);
          break;
        case rosidl_typesupport_introspection_c__ROS_TYPE_BYTE:
          actual_data_size += sizeof(uint8_t);
          break;
        case rosidl_typesupport_introspection_c__ROS_TYPE_CHAR:
        case rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT:
          actual_data_size += sizeof(float);
          break;
        case rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE:
          actual_data_size += sizeof(double);
          break;
        case rosidl_typesupport_introspection_c__ROS_TYPE_LONG_DOUBLE:
          actual_data_size += sizeof(long double);
          break;
        case rosidl_typesupport_introspection_c__ROS_TYPE_INT8:
          actual_data_size += sizeof(int8_t);
          break;
        case rosidl_typesupport_introspection_c__ROS_TYPE_INT16:
          actual_data_size += sizeof(int16_t);
          break;
        case rosidl_typesupport_introspection_c__ROS_TYPE_INT32:
          actual_data_size += sizeof(int32_t);
          break;
        case rosidl_typesupport_introspection_c__ROS_TYPE_INT64:
          actual_data_size += sizeof(int64_t);
          break;
        case rosidl_typesupport_introspection_c__ROS_TYPE_UINT8:
          actual_data_size += sizeof(uint8_t);
          break;
        case rosidl_typesupport_introspection_c__ROS_TYPE_UINT16:
          actual_data_size += sizeof(uint16_t);
          break;
        case rosidl_typesupport_introspection_c__ROS_TYPE_UINT32:
          actual_data_size += sizeof(uint32_t);
          break;
        case rosidl_typesupport_introspection_c__ROS_TYPE_UINT64:
          actual_data_size += sizeof(uint64_t);
	  break;
        case rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE:
	  {
            auto msg_members = GetMembers(member);
	    AnalyzeType(msg_members);
	    if(IsMemcopyable(msg_members))
	    {
              actual_data_size += msg_members->size_of_;
	    }
	    else
	    {
              memcpyable = false;
	    }
	  }
          break;
          //not documented
        case rosidl_typesupport_introspection_c__ROS_TYPE_WSTRING:
        case rosidl_typesupport_introspection_c__ROS_TYPE_WCHAR:
          throw std::logic_error("Wide character/string serialization is unsupported.");
	}
      }

      SetAnalyzed(members);
      if(actual_data_size == struct_size && memcpyable)
      {
	SetMemcopyable(members);
      }
    }
    }
  } // namespace rmw
} // namespace eCAL
