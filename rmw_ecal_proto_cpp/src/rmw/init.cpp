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

#include <rmw/init.h>

#include <rmw/rmw.h>

#include <rmw_ecal_shared_cpp/rmw/init.hpp>

rmw_ret_t rmw_init(const rmw_init_options_t *options, rmw_context_t *context)
{
  return eCAL::rmw::rmw_init(::rmw_get_implementation_identifier(), options, context);
}

rmw_ret_t rmw_shutdown(rmw_context_t *context)
{
  return eCAL::rmw::rmw_shutdown(::rmw_get_implementation_identifier(), context);
}

rmw_ret_t rmw_context_fini(rmw_context_t *context)
{
  return eCAL::rmw::rmw_context_fini(::rmw_get_implementation_identifier(), context);
}