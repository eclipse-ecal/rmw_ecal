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

#include <rmw_ecal_shared_cpp/ros_array_iterator.hpp>

namespace eCAL
{
  namespace rmw
  {

    class Node;
    class GuardCondition;
    class WaitSet;
    class Publisher;
    class Service;
    class Subscriber;
    class Client;
    class Event;

    //Represents client/service sequence number.
    using sequence_number_t = int64_t;

    inline Node *GetImplementation(const rmw_node_t *node)
    {
      return static_cast<Node *>(node->data);
    }

    inline GuardCondition *GetImplementation(const rmw_guard_condition_t *guard_condition)
    {
      return static_cast<GuardCondition *>(guard_condition->data);
    }

    inline WaitSet *GetImplementation(const rmw_wait_set_t *wait_set)
    {
      return static_cast<WaitSet *>(wait_set->data);
    }

    inline Publisher *GetImplementation(const rmw_publisher_t *publisher)
    {
      return static_cast<Publisher *>(publisher->data);
    }

    inline Subscriber *GetImplementation(const rmw_subscription_t *subscription)
    {
      return static_cast<Subscriber *>(subscription->data);
    }

    inline Service *GetImplementation(const rmw_service_t *service)
    {
      return static_cast<Service *>(service->data);
    }

    inline Client *GetImplementation(const rmw_client_t *client)
    {
      return static_cast<Client *>(client->data);
    }

    inline Event *GetImplementation(const rmw_event_t *event)
    {
      return static_cast<Event *>(event->data);
    }

    inline GuardCondition *GetImplementation(const rmw_guard_conditions_t *guard_conditions, size_t index)
    {
      return static_cast<GuardCondition *>(guard_conditions->guard_conditions[index]);
    }

    inline Subscriber *GetImplementation(const rmw_subscriptions_t *subscriptions, size_t index)
    {
      return static_cast<Subscriber *>(subscriptions->subscribers[index]);
    }

    inline Service *GetImplementation(const rmw_services_t *services, size_t index)
    {
      return static_cast<Service *>(services->services[index]);
    }

    inline Client *GetImplementation(const rmw_clients_t *clients, size_t index)
    {
      return static_cast<Client *>(clients->clients[index]);
    }

    inline Event *GetImplementation(const rmw_events_t *events, size_t index)
    {
      return static_cast<Event *>(events->events[index]);
    }

    inline sequence_number_t GenerateSequenceNumber()
    {
      static std::atomic<sequence_number_t> sequence_number{0};
      return sequence_number++;
    }

    inline size_t GetSequenceDataSize(const std::string &sequence)
    {
      return sequence.size() - sizeof(sequence_number_t);
    }

    inline sequence_number_t GetSequenceNumber(const std::string &sequence)
    {
      return *reinterpret_cast<const sequence_number_t *>(sequence.data() + GetSequenceDataSize(sequence));
    }

#define UNIMPLEMENTED                                                                 \
  RMW_SET_ERROR_MSG_WITH_FORMAT_STRING("%s has not been implemented yet.", __func__); \
  return RMW_RET_UNSUPPORTED

#define UNSUPPORTED                                                       \
  RMW_SET_ERROR_MSG_WITH_FORMAT_STRING("%s is not supported.", __func__); \
  return RMW_RET_UNSUPPORTED

#define CHECK_RMW_IMPLEMENTATION(ImplementationIdentifier, ElementName)     \
  CHECK_RMW_IMPLEMENTATION_RET_VALUE(ImplementationIdentifier, ElementName, \
                                     RMW_RET_INCORRECT_RMW_IMPLEMENTATION)

#define CHECK_RMW_IMPLEMENTATION_RET_VALUE(ImplementationIdentifier, ElementName, ReturnValue) \
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(                                                            \
      ElementName,                                                                             \
      ElementName->implementation_identifier,                                                  \
      ImplementationIdentifier,                                                                \
      return ReturnValue)

  } // namespace rmw
} // namespace eCAL
