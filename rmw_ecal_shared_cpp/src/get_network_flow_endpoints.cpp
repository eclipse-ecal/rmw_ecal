// Copyright 2021 Continental AG
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

#include "rmw_ecal_shared_cpp/rmw/get_network_flow_endpoints.hpp"

#include "internal/common.hpp"

namespace eCAL
{
  namespace rmw
  {

    rmw_ret_t rmw_publisher_get_network_flow_endpoints(const rmw_publisher_t * /* publisher */,
                                                       rcutils_allocator_t * /* allocator */,
                                                       rmw_network_flow_endpoint_array_t * /* network_flow_endpoint_array */)
    {
      UNSUPPORTED;
    }

    rmw_ret_t rmw_subscription_get_network_flow_endpoints(const rmw_subscription_t * /* subscription */,
                                                          rcutils_allocator_t * /* allocator */,
                                                          rmw_network_flow_endpoint_array_t * /* network_flow_endpoint_array */)
    {
      UNSUPPORTED;
    }

  } // namespace rmw
} // namespace eCAL

#endif
