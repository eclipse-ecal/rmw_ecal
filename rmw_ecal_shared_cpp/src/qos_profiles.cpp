
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

#if ROS_DISTRO >= GALACTIC

#include <rmw/qos_profiles.h>

#include <rmw_ecal_shared_cpp/visibility.h>
#include <rmw_ecal_shared_cpp/rmw/qos_profiles.hpp>

#include "internal/common.hpp"

namespace eCAL
{
  namespace rmw
  {
    rmw_ret_t rmw_qos_profile_check_compatible(const rmw_qos_profile_t publisher_profile,
                                               const rmw_qos_profile_t subscription_profile,
                                               rmw_qos_compatibility_type_t *compatibility,
                                               char *reason,
                                               size_t reason_size)
    {
      *compatibility = RMW_QOS_COMPATIBILITY_OK;

      if (publisher_profile.avoid_ros_namespace_conventions != subscription_profile.avoid_ros_namespace_conventions)
      {
        std::string ns_conventions_error{ "avoid_ros_namespace_conventions must be same" };
        if (reason_size >= ns_conventions_error.size())
        {
          std::strcpy(reason, ns_conventions_error.c_str());
        }
        *compatibility = RMW_QOS_COMPATIBILITY_ERROR;
      }
      return RMW_RET_OK;
    }
  }
}
#endif
