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

#include <rmw/get_topic_endpoint_info.h>

#include <rmw/rmw.h>

#include <rmw_ecal_shared_cpp/rmw/get_topic_endpoint_info.hpp>

rmw_ret_t rmw_get_publishers_info_by_topic(const rmw_node_t *node, rcutils_allocator_t *allocator,
                                           const char *topic_name, bool no_mangle,
                                           rmw_topic_endpoint_info_array_t *publishers_info)
{
  return eCAL::rmw::rmw_get_publishers_info_by_topic(::rmw_get_implementation_identifier(), node, allocator, topic_name, no_mangle, publishers_info);
}

rmw_ret_t rmw_get_subscriptions_info_by_topic(const rmw_node_t *node, rcutils_allocator_t *allocator,
                                              const char *topic_name, bool no_mangle,
                                              rmw_topic_endpoint_info_array_t *subscriptions_info)
{
  return eCAL::rmw::rmw_get_subscriptions_info_by_topic(::rmw_get_implementation_identifier(), node, allocator, topic_name, no_mangle, subscriptions_info);
}
