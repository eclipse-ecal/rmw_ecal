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

#include <rmw/init_options.h>

#include <rmw_ecal_shared_cpp/visibility.h>

namespace eCAL
{
  namespace rmw
  {
    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_init_options_init(const char *implementation_identifier, rmw_init_options_t *init_options, rcutils_allocator_t allocator);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_init_options_copy(const char *implementation_identifier, const rmw_init_options_t *src, rmw_init_options_t *dst);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_init_options_fini(const char *implementation_identifier, rmw_init_options_t *init_options);

  } // namespace rmw
} // namespace eCAL