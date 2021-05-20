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

#include <rmw/get_service_names_and_types.h>

#include <rmw_ecal_shared_cpp/visibility.h>

namespace eCAL
{
  namespace rmw
  {
    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_get_service_names_and_types(const char *implementation_identifier,
                                              const rmw_node_t *node,
                                              rcutils_allocator_t *allocator,
                                              rmw_names_and_types_t *service_names_and_types);

  } // namespace rmw
} // namespace eCAL