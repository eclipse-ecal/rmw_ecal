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

#include <string>
#include <cstring>
#include <atomic>

#include <rmw/types.h>
#include <rmw/event.h>
#include <rmw/ret_types.h>
#include <rmw/error_handling.h>
#include <rmw/impl/cpp/macros.hpp>

#include <ecal/ecal.h>
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4146 4800)
#endif
#include <ecal/pb/monitoring.pb.h>
#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include <rmw_ecal_shared_cpp/ros_array_iterator.hpp>

namespace eCAL
{
  namespace rmw
  {
    inline char *ConstructCString(const char *value)
    {
      size_t src_len{std::strlen(value) + 1};
      char *dest = new char[src_len];
      std::strcpy(dest, value);
      return dest;
    }

    inline std::string ReplaceString(std::string subject, const std::string &search,
                                     const std::string &replace)
    {
      size_t pos = 0;
      while ((pos = subject.find(search, pos)) != std::string::npos)
      {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
      }
      return subject;
    }

    inline char *ConstructCString(const std::string &value)
    {
      return ConstructCString(value.c_str());
    }

  } // namespace rmw
} // namespace eCAL
