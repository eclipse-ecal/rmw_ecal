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

#include <rmw/event.h>

#include <rmw_ecal_shared_cpp/visibility.h>

namespace eCAL
{
  namespace rmw
  {
    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_take_event(const char *implementation_identifier,
                             const rmw_event_t *event_handle,
                             void *event_info,
                             bool *taken);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_publisher_event_init(const char *implementation_identifier,
                                       rmw_event_t *rmw_event,
                                       const rmw_publisher_t *publisher,
                                       rmw_event_type_t event_type);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_subscription_event_init(const char *implementation_identifier,
                                          rmw_event_t *rmw_event,
                                          const rmw_subscription_t *subscription,
                                          rmw_event_type_t event_type);

  } // namespace rmw
} // namespace eCAL