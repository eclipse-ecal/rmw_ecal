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

#include<rmw/rmw.h>

#include <rmw_ecal_shared_cpp/rmw/init_options.hpp>

rmw_ret_t rmw_init_options_init(rmw_init_options_t *init_options, rcutils_allocator_t allocator)
{
  return eCAL::rmw::rmw_init_options_init(::rmw_get_implementation_identifier(), init_options, allocator);
}

rmw_ret_t rmw_init_options_copy(const rmw_init_options_t *src, rmw_init_options_t *dst)
{
  return eCAL::rmw::rmw_init_options_copy(::rmw_get_implementation_identifier(), src, dst);
}

rmw_ret_t rmw_init_options_fini(rmw_init_options_t *init_options)
{
  return eCAL::rmw::rmw_init_options_fini(::rmw_get_implementation_identifier(), init_options);
}