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

#include "rmw_ecal_shared_cpp/rmw/rmw.hpp"

#include <algorithm>
#include <iterator>
#include <chrono>
#include <memory>
#include <mutex>

#include <ecal/ecal.h>

#include <rmw/impl/cpp/macros.hpp>

#include "rmw_ecal_shared_cpp/string_functions.hpp"

#include "internal/common.hpp"
#include "internal/node.hpp"
#include "internal/wait_set.hpp"
#include "internal/subscriber.hpp"
#include "internal/publisher.hpp"
#include "internal/service.hpp"
#include "internal/client.hpp"
#include "internal/guard_condition.hpp"
#include "internal/graph.hpp"

namespace eCAL
{
  namespace rmw
  {

#if ROS_DISTRO >= GALACTIC
    rmw_node_t *rmw_create_node(const char *implementation_identifier,
                                rmw_context_t *context,
                                const char *name,
                                const char *namespace_)
#else
    rmw_node_t *rmw_create_node(const char *implementation_identifier,
                                rmw_context_t *context,
                                const char *name,
                                const char *namespace_,
                                size_t /* domain_id */,
                                bool /* localhost_only */)
#endif
    {
      RMW_CHECK_ARGUMENT_FOR_NULL(context, nullptr);
      RMW_CHECK_ARGUMENT_FOR_NULL(name, nullptr);
      RMW_CHECK_ARGUMENT_FOR_NULL(namespace_, nullptr);
      CHECK_RMW_IMPLEMENTATION_RET_VALUE(implementation_identifier, context, nullptr);

      auto rmw_node = rmw_node_allocate();
      auto ecal_node = Graph::CreateNode(namespace_, name);

      rmw_node->context = context;
      rmw_node->implementation_identifier = implementation_identifier;
      rmw_node->name = ConstructCString(name);
      rmw_node->namespace_ = ConstructCString(namespace_);
      rmw_node->data = ecal_node;
      ecal_node->guard_condition = rmw_create_guard_condition(implementation_identifier, rmw_node->context);

      return rmw_node;
    }

    rmw_ret_t rmw_destroy_node(const char *implementation_identifier, rmw_node_t *node)
    {
      RMW_CHECK_ARGUMENT_FOR_NULL(node, RMW_RET_INVALID_ARGUMENT);
      CHECK_RMW_IMPLEMENTATION(implementation_identifier, node);

      auto ecal_node = GetImplementation(node);
      rmw_guard_condition_free(ecal_node->guard_condition);
      Graph::DestroyNode(ecal_node);
      delete[] node->name;
      delete[] node->namespace_;

      ::rmw_node_free(node);

      return RMW_RET_OK;
    }

    rmw_ret_t rmw_node_assert_liveliness(const char * /* implementation_identifier */, const rmw_node_t * /* node */)
    {
      UNSUPPORTED;
    }

    const rmw_guard_condition_t *rmw_node_get_graph_guard_condition(const char *implementation_identifier, const rmw_node_t *node)
    {
      RMW_CHECK_ARGUMENT_FOR_NULL(node, nullptr);
      CHECK_RMW_IMPLEMENTATION_RET_VALUE(implementation_identifier, node, nullptr);

      return GetImplementation(node)->guard_condition;
    }

    rmw_publisher_t *rmw_create_publisher(const char *implementation_identifier,
                                          const TypesupportFactory &ecal_typesupport_factory,
                                          const rmw_node_t *node,
                                          const rosidl_message_type_support_t *type_support,
                                          const char *topic_name,
                                          const rmw_qos_profile_t *qos_policies,
                                          const rmw_publisher_options_t *publisher_options)
    {
      RMW_CHECK_ARGUMENT_FOR_NULL(node, nullptr);
      RMW_CHECK_ARGUMENT_FOR_NULL(type_support, nullptr);
      RMW_CHECK_ARGUMENT_FOR_NULL(topic_name, nullptr);
      RMW_CHECK_ARGUMENT_FOR_NULL(qos_policies, nullptr);
      RMW_CHECK_ARGUMENT_FOR_NULL(publisher_options, nullptr);
      CHECK_RMW_IMPLEMENTATION_RET_VALUE(implementation_identifier, node, nullptr);

      auto ecal_node = GetImplementation(node);
      auto ecal_ts = ecal_typesupport_factory.Create(type_support);
      auto ecal_qos = CreatePublisherQOS(qos_policies);
      auto ecal_pub = ecal_node->CreatePublisher(topic_name, ecal_ts, ecal_qos);

      auto rmw_pub = ::rmw_publisher_allocate();
      rmw_pub->implementation_identifier = implementation_identifier;
      rmw_pub->topic_name = ConstructCString(topic_name);
      rmw_pub->data = ecal_pub;
      rmw_pub->options = *publisher_options;
      rmw_pub->can_loan_messages = false;

      return rmw_pub;
    }

    rmw_ret_t rmw_destroy_publisher(const char *implementation_identifier, rmw_node_t *node, rmw_publisher_t *publisher)
    {
      RMW_CHECK_ARGUMENT_FOR_NULL(node, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(publisher, RMW_RET_INVALID_ARGUMENT);
      CHECK_RMW_IMPLEMENTATION(implementation_identifier, node);

      auto ecal_pub = GetImplementation(publisher);
      GetImplementation(node)->DestroyPublisher(ecal_pub);
      delete[] publisher->topic_name;

      ::rmw_publisher_free(publisher);

      return RMW_RET_OK;
    }

    rmw_ret_t rmw_publish(const char *implementation_identifier,
                          const rmw_publisher_t *publisher,
                          const void *ros_message,
                          rmw_publisher_allocation_t * /* allocation */)
    {
      RMW_CHECK_ARGUMENT_FOR_NULL(publisher, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(ros_message, RMW_RET_INVALID_ARGUMENT);
      CHECK_RMW_IMPLEMENTATION(implementation_identifier, publisher);

      GetImplementation(publisher)->Publish(ros_message);

      return RMW_RET_OK;
    }

    rmw_ret_t rmw_publisher_count_matched_subscriptions(const char *implementation_identifier,
                                                        const rmw_publisher_t *publisher,
                                                        size_t *subscription_count)
    {
      RMW_CHECK_ARGUMENT_FOR_NULL(publisher, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(subscription_count, RMW_RET_INVALID_ARGUMENT);
      CHECK_RMW_IMPLEMENTATION(implementation_identifier, publisher);

      *subscription_count = GetImplementation(publisher)->CountSubscribers();

      return RMW_RET_OK;
    }

    rmw_ret_t rmw_publisher_get_actual_qos(const char *implementation_identifier,
                                           const rmw_publisher_t *publisher,
                                           rmw_qos_profile_t *qos)
    {
      RMW_CHECK_ARGUMENT_FOR_NULL(publisher, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(qos, RMW_RET_INVALID_ARGUMENT);
      CHECK_RMW_IMPLEMENTATION(implementation_identifier, publisher);

      auto ecal_pub = GetImplementation(publisher);
      *qos = ecal_pub->GetRosQOSProfile();

      return RMW_RET_OK;
    }

    rmw_ret_t rmw_publish_serialized_message(const char *implementation_identifier,
                                             const rmw_publisher_t *publisher,
                                             const rmw_serialized_message_t *serialized_message,
                                             rmw_publisher_allocation_t * /* allocation */)
    {
      RMW_CHECK_ARGUMENT_FOR_NULL(publisher, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(serialized_message, RMW_RET_INVALID_ARGUMENT);
      CHECK_RMW_IMPLEMENTATION(implementation_identifier, publisher);

      auto ecal_pub = GetImplementation(publisher);
      ecal_pub->PublishRaw(serialized_message->buffer, serialized_message->buffer_length);

      return RMW_RET_OK;
    }

    rmw_ret_t rmw_get_serialized_message_size(const char * /* implementation_identifier */,
                                              const rosidl_message_type_support_t * /* type_support */,
                                              const rosidl_runtime_c__Sequence__bound * /* message_bounds */,
                                              size_t * /* size */)
    {
      UNSUPPORTED;
    }

    rmw_ret_t rmw_serialize(const char * /* implementation_identifier */,
                            const SerializerFactory &ecal_serializer_factory,
                            const void *ros_message,
                            const rosidl_message_type_support_t *type_support,
                            rmw_serialized_message_t *serialized_message)
    {
      RMW_CHECK_ARGUMENT_FOR_NULL(ros_message, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(type_support, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(serialized_message, RMW_RET_INVALID_ARGUMENT);

      std::unique_ptr<Serializer> ecal_ser{ecal_serializer_factory.CreateSerializer(type_support)};
      auto serialized_bytes = ecal_ser->Serialize(ros_message);
      auto no_of_bytes = serialized_bytes.size();

      serialized_message->buffer_capacity = no_of_bytes;
      serialized_message->buffer_length = no_of_bytes;
      serialized_message->buffer = new uint8_t[no_of_bytes];
      //not very efficient, should find a way to avoid this copy
      std::copy_n(serialized_bytes.data(), no_of_bytes, serialized_message->buffer);

      return RMW_RET_OK;
    }

    rmw_ret_t rmw_deserialize(const char * /* implementation_identifier */,
                              const SerializerFactory &ecal_serializer_factory,
                              const rmw_serialized_message_t *serialized_message,
                              const rosidl_message_type_support_t *type_support,
                              void *ros_message)
    {
      std::unique_ptr<Deserializer> ecal_deser{ecal_serializer_factory.CreateDeserializer(type_support)};
      ecal_deser->Deserialize(ros_message, serialized_message->buffer, serialized_message->buffer_length);

      return RMW_RET_OK;
    }

    rmw_subscription_t *rmw_create_subscription(const char *implementation_identifier,
                                                const TypesupportFactory &ecal_typesupport_factory,
                                                const rmw_node_t *node,
                                                const rosidl_message_type_support_t *type_support,
                                                const char *topic_name,
                                                const rmw_qos_profile_t *qos_policies,
                                                const rmw_subscription_options_t *subscription_options)
    {
      RMW_CHECK_ARGUMENT_FOR_NULL(node, nullptr);
      RMW_CHECK_ARGUMENT_FOR_NULL(topic_name, nullptr);
      RMW_CHECK_ARGUMENT_FOR_NULL(type_support, nullptr);
      RMW_CHECK_ARGUMENT_FOR_NULL(qos_policies, nullptr);
      RMW_CHECK_ARGUMENT_FOR_NULL(subscription_options, nullptr);
      CHECK_RMW_IMPLEMENTATION_RET_VALUE(implementation_identifier, node, nullptr);

      auto ecal_node = GetImplementation(node);
      auto ecal_ts = ecal_typesupport_factory.Create(type_support);
      auto ecal_qos = CreateSubscriberQOS(qos_policies);
      auto ecal_sub = ecal_node->CreateSubscriber(topic_name, ecal_ts, ecal_qos);

      auto rmw_sub = ::rmw_subscription_allocate();
      rmw_sub->implementation_identifier = implementation_identifier;
      rmw_sub->topic_name = ConstructCString(topic_name);
      rmw_sub->data = ecal_sub;
      rmw_sub->can_loan_messages = false;

      return rmw_sub;
    }

    rmw_ret_t rmw_destroy_subscription(const char *implementation_identifier, rmw_node_t *node, rmw_subscription_t *subscription)
    {
      RMW_CHECK_ARGUMENT_FOR_NULL(node, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(subscription, RMW_RET_INVALID_ARGUMENT);
      CHECK_RMW_IMPLEMENTATION(implementation_identifier, node);

      auto ecal_sub = GetImplementation(subscription);
      GetImplementation(node)->DestroySubscriber(ecal_sub);
      delete[] subscription->topic_name;

      ::rmw_subscription_free(subscription);

      return RMW_RET_OK;
    }

    rmw_ret_t rmw_subscription_count_matched_publishers(const char *implementation_identifier,
                                                        const rmw_subscription_t *subscription,
                                                        size_t *publisher_count)
    {
      RMW_CHECK_ARGUMENT_FOR_NULL(subscription, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(publisher_count, RMW_RET_INVALID_ARGUMENT);
      CHECK_RMW_IMPLEMENTATION(implementation_identifier, subscription);

      *publisher_count = GetImplementation(subscription)->CountPublishers();

      return RMW_RET_OK;
    }

    rmw_ret_t rmw_take(const char *implementation_identifier,
                       const rmw_subscription_t *subscription,
                       void *ros_message,
                       bool *taken,
                       rmw_subscription_allocation_t * /* allocation */)
    {
      RMW_CHECK_ARGUMENT_FOR_NULL(subscription, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(ros_message, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(taken, RMW_RET_INVALID_ARGUMENT);
      CHECK_RMW_IMPLEMENTATION(implementation_identifier, subscription);

      auto ecal_sub = GetImplementation(subscription);
      if (!ecal_sub->HasData())
        return RMW_RET_OK;
      ecal_sub->TakeLatestData(ros_message);
      *taken = true;

      return RMW_RET_OK;
    }

    rmw_ret_t rmw_take_with_info(const char *implementation_identifier,
                                 const rmw_subscription_t *subscription,
                                 void *ros_message,
                                 bool *taken,
                                 rmw_message_info_t *message_info,
                                 rmw_subscription_allocation_t * /* allocation */)
    {
      RMW_CHECK_ARGUMENT_FOR_NULL(subscription, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(ros_message, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(taken, RMW_RET_INVALID_ARGUMENT);
      CHECK_RMW_IMPLEMENTATION(implementation_identifier, subscription);

      auto ecal_sub = GetImplementation(subscription);
      if (!ecal_sub->HasData())
        return RMW_RET_OK;
      auto ecal_msg_info = ecal_sub->TakeLatestDataWithInfo(ros_message);
      // eCAL timestamps are in microseconds but ROS expects them in nanoseconds
      std::chrono::microseconds src_ts_ms{ecal_msg_info.send_timestamp};
      std::chrono::microseconds rcv_ts_ms{ecal_msg_info.receive_timestamp};
      message_info->source_timestamp =
        std::chrono::duration_cast<std::chrono::nanoseconds>(src_ts_ms).count();
      message_info->received_timestamp =
        std::chrono::duration_cast<std::chrono::nanoseconds>(rcv_ts_ms).count();
      *taken = true;

      return RMW_RET_OK;
    }

    rmw_ret_t rmw_take_sequence(const char *implementation_identifier,
                                const rmw_subscription_t *subscription,
                                size_t count,
                                rmw_message_sequence_t *message_sequence,
                                rmw_message_info_sequence_t *message_info_sequence,
                                size_t *taken,
                                rmw_subscription_allocation_t * /* allocation */)
    {
      RMW_CHECK_ARGUMENT_FOR_NULL(subscription, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(message_sequence, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(message_info_sequence, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(taken, RMW_RET_INVALID_ARGUMENT);
      CHECK_RMW_IMPLEMENTATION(implementation_identifier, subscription);

      if (message_sequence->capacity < count)
      {
        RMW_SET_ERROR_MSG("message_sequence capacity is smaller then count.");
        return RMW_RET_INVALID_ARGUMENT;
      }
      if (message_info_sequence->capacity < count)
      {
        RMW_SET_ERROR_MSG("message_info_sequence capacity is smaller then count.");
        return RMW_RET_INVALID_ARGUMENT;
      }

      *taken = 0;
      auto ecal_sub = GetImplementation(subscription);
      while (ecal_sub->HasData() && *taken != count)
      {
        ecal_sub->TakeLatestData(message_sequence->data[*taken]);
        (*taken)++;
      }

      return RMW_RET_OK;
    }

    rmw_ret_t rmw_take_serialized_message(const char *implementation_identifier,
                                          const rmw_subscription_t *subscription,
                                          rmw_serialized_message_t *serialized_message,
                                          bool *taken,
                                          rmw_subscription_allocation_t * /* allocation */)
    {
      RMW_CHECK_ARGUMENT_FOR_NULL(subscription, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(serialized_message, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(taken, RMW_RET_INVALID_ARGUMENT);
      CHECK_RMW_IMPLEMENTATION(implementation_identifier, subscription);

      auto ecal_sub = GetImplementation(subscription);
      if (!ecal_sub->HasData())
        return RMW_RET_OK;

      auto data = ecal_sub->TakeLatestSerializedData();
      serialized_message->buffer = reinterpret_cast<uint8_t *>(data.data);
      serialized_message->buffer_length = data.size;
      serialized_message->buffer_capacity = data.size;
      *taken = true;

      return RMW_RET_OK;
    }

    rmw_ret_t rmw_take_serialized_message_with_info(const char *implementation_identifier,
                                                    const rmw_subscription_t *subscription,
                                                    rmw_serialized_message_t *serialized_message,
                                                    bool *taken,
                                                    rmw_message_info_t * /* message_info */,
                                                    rmw_subscription_allocation_t *allocation)
    {
      return rmw_take_serialized_message(implementation_identifier, subscription, serialized_message, taken, allocation);
    }

    rmw_client_t *rmw_create_client(const char *implementation_identifier,
                                    const TypesupportFactory &ecal_typesupport_factory,
                                    const rmw_node_t *node,
                                    const rosidl_service_type_support_t *type_support,
                                    const char *service_name,
                                    const rmw_qos_profile_t *qos_policies)
    {
      RMW_CHECK_ARGUMENT_FOR_NULL(node, nullptr);
      RMW_CHECK_ARGUMENT_FOR_NULL(type_support, nullptr);
      RMW_CHECK_ARGUMENT_FOR_NULL(service_name, nullptr);
      RMW_CHECK_ARGUMENT_FOR_NULL(qos_policies, nullptr);
      CHECK_RMW_IMPLEMENTATION_RET_VALUE(implementation_identifier, node, nullptr);

      auto ecal_node = GetImplementation(node);
      auto ecal_ts = ecal_typesupport_factory.Create(type_support);
      auto ecal_qos = CreateClientQOS(qos_policies);
      auto ecal_client = ecal_node->CreateClient(service_name, ecal_ts, ecal_qos);

      auto rmw_client = ::rmw_client_allocate();
      rmw_client->data = ecal_client;
      rmw_client->implementation_identifier = implementation_identifier;
      rmw_client->service_name = ConstructCString(service_name);

      return rmw_client;
    }

    rmw_ret_t rmw_destroy_client(const char *implementation_identifier, rmw_node_t *node, rmw_client_t *client)
    {
      RMW_CHECK_ARGUMENT_FOR_NULL(node, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(client, RMW_RET_INVALID_ARGUMENT);
      CHECK_RMW_IMPLEMENTATION(implementation_identifier, node);

      auto ecal_client = GetImplementation(client);
      GetImplementation(node)->DestroyClient(ecal_client);
      delete[] client->service_name;

      ::rmw_client_free(client);

      return RMW_RET_OK;
    }

    rmw_ret_t rmw_send_request(const char *implementation_identifier, const rmw_client_t *client,
                               const void *ros_request,
                               int64_t *sequence_id)
    {
      RMW_CHECK_ARGUMENT_FOR_NULL(client, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(ros_request, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(sequence_id, RMW_RET_INVALID_ARGUMENT);
      CHECK_RMW_IMPLEMENTATION(implementation_identifier, client);

      auto ecal_client = GetImplementation(client);
      *sequence_id = ecal_client->SendRequest(ros_request);
      if (*sequence_id == -1)
        return RMW_RET_ERROR;

      return RMW_RET_OK;
    }

    rmw_ret_t rmw_take_response(const char *implementation_identifier,
                                const rmw_client_t *client,
                                rmw_service_info_t *request_header,
                                void *ros_response,
                                bool *taken)
    {
      RMW_CHECK_ARGUMENT_FOR_NULL(client, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(request_header, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(ros_response, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(taken, RMW_RET_INVALID_ARGUMENT);
      CHECK_RMW_IMPLEMENTATION(implementation_identifier, client);

      auto ecal_client = GetImplementation(client);
      if (ecal_client->HasResponse())
      {
        request_header->request_id.sequence_number = ecal_client->TakeResponse(ros_response);
        *taken = true;
      }

      return RMW_RET_OK;
    }

    rmw_service_t *rmw_create_service(const char *implementation_identifier,
                                      const TypesupportFactory &ecal_typesupport_factory,
                                      const rmw_node_t *node,
                                      const rosidl_service_type_support_t *type_support,
                                      const char *service_name,
                                      const rmw_qos_profile_t *qos_policies)
    {
      RMW_CHECK_ARGUMENT_FOR_NULL(node, nullptr);
      RMW_CHECK_ARGUMENT_FOR_NULL(type_support, nullptr);
      RMW_CHECK_ARGUMENT_FOR_NULL(service_name, nullptr);
      RMW_CHECK_ARGUMENT_FOR_NULL(qos_policies, nullptr);
      CHECK_RMW_IMPLEMENTATION_RET_VALUE(implementation_identifier, node, nullptr);

      auto ecal_node = GetImplementation(node);
      auto ecal_ts = ecal_typesupport_factory.Create(type_support);
      auto ecal_qos = CreateServiceQOS(qos_policies);
      auto ecal_service = ecal_node->CreateService(service_name, ecal_ts, ecal_qos);

      auto rmw_service = ::rmw_service_allocate();
      rmw_service->data = ecal_service;
      rmw_service->implementation_identifier = implementation_identifier;
      rmw_service->service_name = ConstructCString(service_name);

      return rmw_service;
    }

    rmw_ret_t rmw_destroy_service(const char *implementation_identifier, rmw_node_t *node, rmw_service_t *service)
    {
      RMW_CHECK_ARGUMENT_FOR_NULL(node, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(service, RMW_RET_INVALID_ARGUMENT);
      CHECK_RMW_IMPLEMENTATION(implementation_identifier, node);

      auto ecal_service = GetImplementation(service);
      GetImplementation(node)->DestroyService(ecal_service);
      delete[] service->service_name;

      ::rmw_service_free(service);

      return RMW_RET_OK;
    }

    rmw_ret_t rmw_take_request(const char *implementation_identifier,
                               const rmw_service_t *service,
                               rmw_service_info_t *request_header,
                               void *ros_request,
                               bool *taken)
    {
      RMW_CHECK_ARGUMENT_FOR_NULL(service, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(request_header, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(ros_request, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(taken, RMW_RET_INVALID_ARGUMENT);
      CHECK_RMW_IMPLEMENTATION(implementation_identifier, service);

      auto ecal_service = GetImplementation(service);
      if (ecal_service->HasRequest())
      {
        request_header->request_id.sequence_number = ecal_service->TakeRequest(ros_request);
        *taken = true;
      }

      return RMW_RET_OK;
    }

    rmw_ret_t rmw_send_response(const char *implementation_identifier,
                                const rmw_service_t *service,
                                rmw_request_id_t *request_header,
                                void *ros_response)
    {
      RMW_CHECK_ARGUMENT_FOR_NULL(service, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(request_header, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(ros_response, RMW_RET_INVALID_ARGUMENT);
      CHECK_RMW_IMPLEMENTATION(implementation_identifier, service);

      auto ecal_service = GetImplementation(service);
      ecal_service->SendResponse(ros_response, request_header->sequence_number);

      return RMW_RET_OK;
    }

    rmw_guard_condition_t *rmw_create_guard_condition(const char *implementation_identifier, rmw_context_t *context)
    {
      RMW_CHECK_ARGUMENT_FOR_NULL(context, nullptr);
      CHECK_RMW_IMPLEMENTATION_RET_VALUE(implementation_identifier, context, nullptr);

      auto rmw_gc = ::rmw_guard_condition_allocate();
      rmw_gc->context = context;
      rmw_gc->data = new GuardCondition;
      rmw_gc->implementation_identifier = implementation_identifier;

      return rmw_gc;
    }

    rmw_ret_t rmw_destroy_guard_condition(const char *implementation_identifier, rmw_guard_condition_t *guard_condition)
    {
      RMW_CHECK_ARGUMENT_FOR_NULL(guard_condition, RMW_RET_INVALID_ARGUMENT);
      CHECK_RMW_IMPLEMENTATION(implementation_identifier, guard_condition);

      delete GetImplementation(guard_condition);
      ::rmw_guard_condition_free(guard_condition);

      return RMW_RET_OK;
    }

    rmw_ret_t rmw_trigger_guard_condition(const char *implementation_identifier, const rmw_guard_condition_t *guard_condition)
    {
      RMW_CHECK_ARGUMENT_FOR_NULL(guard_condition, RMW_RET_INVALID_ARGUMENT);
      CHECK_RMW_IMPLEMENTATION(implementation_identifier, guard_condition);

      GetImplementation(guard_condition)->Trigger();

      return RMW_RET_OK;
    }

    rmw_wait_set_t *rmw_create_wait_set(const char *implementation_identifier, rmw_context_t *context, size_t /* max_conditions */)
    {
      RMW_CHECK_ARGUMENT_FOR_NULL(context, nullptr);
      CHECK_RMW_IMPLEMENTATION_RET_VALUE(implementation_identifier, context, nullptr);

      auto rmw_ws = ::rmw_wait_set_allocate();
      rmw_ws->data = new WaitSet;
      rmw_ws->implementation_identifier = implementation_identifier;

      return rmw_ws;
    }

    rmw_ret_t rmw_destroy_wait_set(const char *implementation_identifier, rmw_wait_set_t *wait_set)
    {
      RMW_CHECK_ARGUMENT_FOR_NULL(wait_set, RMW_RET_INVALID_ARGUMENT);
      CHECK_RMW_IMPLEMENTATION(implementation_identifier, wait_set);

      delete GetImplementation(wait_set);
      ::rmw_wait_set_free(wait_set);

      return RMW_RET_OK;
    }

    bool has_data(rmw_subscriptions_t *subscriptions,
                  rmw_guard_conditions_t *guard_conditions,
                  rmw_services_t *services,
                  rmw_clients_t *clients,
                  rmw_events_t *events)
    {
      for (size_t i = 0; i < subscriptions->subscriber_count; ++i)
      {
        const auto ecal_sub = GetImplementation(subscriptions, i);
        if (ecal_sub->HasData())
        {
          return true;
        }
      }

      for (size_t i = 0; i < guard_conditions->guard_condition_count; ++i)
      {
        const auto ecal_gc = GetImplementation(guard_conditions, i);
        if (ecal_gc->Triggered())
        {
          return true;
        }
      }

      for (size_t i = 0; i < services->service_count; ++i)
      {
        const auto ecal_service = GetImplementation(services, i);
        if (ecal_service->HasRequest())
        {
          return true;
        }
      }

      for (size_t i = 0; i < clients->client_count; ++i)
      {
        const auto ecal_client = GetImplementation(clients, i);
        if (ecal_client->HasResponse())
        {
          return true;
        }
      }

      for (size_t i = 0; i < events->event_count; ++i)
      {
        const auto ecal_event = GetImplementation(events, i);
        if (ecal_event->Triggered())
        {
          return true;
        }
      }

      return false;
    }

    void attach_wait_set(rmw_subscriptions_t *subscriptions,
                         rmw_guard_conditions_t *guard_conditions,
                         rmw_services_t *services,
                         rmw_clients_t *clients,
                         rmw_events_t *events,
                         WaitSet *ecal_ws)
    {
      for (size_t i = 0; i < subscriptions->subscriber_count; ++i)
      {
        auto ecal_sub = GetImplementation(subscriptions, i);
        ecal_sub->AttachWaitSet(ecal_ws);
      }

      for (size_t i = 0; i < guard_conditions->guard_condition_count; ++i)
      {
        auto ecal_gc = GetImplementation(guard_conditions, i);
        ecal_gc->AttachWaitSet(ecal_ws);
      }

      for (size_t i = 0; i < services->service_count; ++i)
      {
        auto ecal_service = GetImplementation(services, i);
        ecal_service->AttachWaitSet(ecal_ws);
      }

      for (size_t i = 0; i < clients->client_count; ++i)
      {
        auto ecal_client = GetImplementation(clients, i);
        ecal_client->AttachWaitSet(ecal_ws);
      }

      for (size_t i = 0; i < events->event_count; ++i)
      {
        auto ecal_event = GetImplementation(events, i);
        ecal_event->AttachWaitSet(ecal_ws);
      }
    }

    void detach_wait_set(rmw_subscriptions_t *subscriptions,
                         rmw_guard_conditions_t *guard_conditions,
                         rmw_services_t *services,
                         rmw_clients_t *clients,
                         rmw_events_t *events)
    {
      for (size_t i = 0; i < subscriptions->subscriber_count; ++i)
      {
        auto ecal_sub = GetImplementation(subscriptions, i);
        ecal_sub->DetachWaitSet();
        if (!ecal_sub->HasData())
        {
          subscriptions->subscribers[i] = nullptr;
        }
      }

      for (size_t i = 0; i < guard_conditions->guard_condition_count; ++i)
      {
        auto ecal_gc = GetImplementation(guard_conditions, i);
        ecal_gc->DetachWaitSet();
        if (!ecal_gc->TakeTriggered())
        {
          guard_conditions->guard_conditions[i] = nullptr;
        }
      }

      for (size_t i = 0; i < services->service_count; ++i)
      {
        auto ecal_ser = GetImplementation(services, i);
        ecal_ser->DetachWaitSet();
        if (!ecal_ser->HasRequest())
        {
          services->services[i] = nullptr;
        }
      }

      for (size_t i = 0; i < clients->client_count; ++i)
      {
        auto ecal_client = GetImplementation(clients, i);
        ecal_client->DetachWaitSet();
        if (!ecal_client->HasResponse())
        {
          clients->clients[i] = nullptr;
        }
      }

      for (size_t i = 0; i < events->event_count; ++i)
      {
        auto ecal_event = GetImplementation(events, i);
        ecal_event->DetachWaitSet();
        if (!ecal_event->Triggered())
        {
          events->events[i] = nullptr;
        }
      }
    }

    rmw_ret_t rmw_wait(const char * /* implementation_identifier */,
                       rmw_subscriptions_t *subscriptions,
                       rmw_guard_conditions_t *guard_conditions,
                       rmw_services_t *services,
                       rmw_clients_t *clients,
                       rmw_events_t *events,
                       rmw_wait_set_t *wait_set,
                       const rmw_time_t *wait_timeout)
    {
      auto ecal_ws = GetImplementation(wait_set);
      bool timed_out = false;

      attach_wait_set(subscriptions, guard_conditions, services, clients, events, ecal_ws);

      std::unique_lock<std::mutex> lock(ecal_ws->condition_mutex);
      while (!has_data(subscriptions, guard_conditions, services, clients, events))
      {
        if (wait_timeout)
        {
          auto n = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(wait_timeout->sec)) + std::chrono::nanoseconds(wait_timeout->nsec);
          timed_out = !ecal_ws->Wait(lock, n);
          if (timed_out)
            break;
        }
        else
        {
          ecal_ws->Wait(lock);
        }
      }
      lock.unlock();

      detach_wait_set(subscriptions, guard_conditions, services, clients, events);

      return timed_out ? RMW_RET_TIMEOUT : RMW_RET_OK;
    }

    rmw_ret_t rmw_get_node_names(const char *implementation_identifier,
                                 const rmw_node_t *node,
                                 rcutils_string_array_t *node_names,
                                 rcutils_string_array_t *node_namespaces)
    {
      RMW_CHECK_ARGUMENT_FOR_NULL(node, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(node_names, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(node_namespaces, RMW_RET_INVALID_ARGUMENT);
      CHECK_RMW_IMPLEMENTATION(implementation_identifier, node);

      auto nodes = Graph::GetNodes();

      auto allocator = rcutils_get_default_allocator();
      auto init_success = rcutils_string_array_init(node_names, nodes.size(), &allocator);
      if (init_success != RMW_RET_OK)
      {
        RMW_SET_ERROR_MSG("Failed to initialize node_names.");
        return RMW_RET_ERROR;
      }
      init_success = rcutils_string_array_init(node_namespaces, nodes.size(), &allocator);
      if (init_success != RMW_RET_OK)
      {
        RMW_SET_ERROR_MSG("Failed to initialize node_namespaces.");
        return RMW_RET_ERROR;
      }

      std::transform(nodes.begin(), nodes.end(), RosArray::Begin(*node_names),
                     [](auto &node) {
                       return ConstructCString(node.name);
                     });
      std::transform(nodes.begin(), nodes.end(), RosArray::Begin(*node_namespaces),
                     [](auto &node) {
                       return ConstructCString(node.name_space);
                     });

      return RMW_RET_OK;
    }

    rmw_ret_t rmw_get_node_names_with_enclaves(const char * /* implementation_identifier */,
                                               const rmw_node_t * /* node */,
                                               rcutils_string_array_t * /* node_names */,
                                               rcutils_string_array_t * /* node_namespaces */,
                                               rcutils_string_array_t * /* enclaves */)
    {
      UNSUPPORTED;
    }

    rmw_ret_t rmw_count_publishers(const char *implementation_identifier,
                                   const rmw_node_t *node,
                                   const char *topic_name,
                                   size_t *count)
    {
      RMW_CHECK_ARGUMENT_FOR_NULL(node, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(topic_name, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(count, RMW_RET_INVALID_ARGUMENT);
      CHECK_RMW_IMPLEMENTATION(implementation_identifier, node);

      *count = Graph::CountPublishers(topic_name);

      return RMW_RET_OK;
    }

    rmw_ret_t rmw_count_subscribers(const char *implementation_identifier,
                                    const rmw_node_t *node,
                                    const char *topic_name,
                                    size_t *count)
    {
      RMW_CHECK_ARGUMENT_FOR_NULL(node, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(topic_name, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(count, RMW_RET_INVALID_ARGUMENT);
      CHECK_RMW_IMPLEMENTATION(implementation_identifier, node);

      *count = Graph::CountSubscribers(topic_name);

      return RMW_RET_OK;
    }

    rmw_ret_t rmw_get_gid_for_publisher(const char *implementation_identifier, const rmw_publisher_t *publisher, rmw_gid_t *gid)
    {
      RMW_CHECK_ARGUMENT_FOR_NULL(publisher, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(gid, RMW_RET_INVALID_ARGUMENT);
      CHECK_RMW_IMPLEMENTATION(implementation_identifier, publisher);

      gid->implementation_identifier = implementation_identifier;
      std::fill(std::begin(gid->data), std::end(gid->data), uint8_t{0});

      return RMW_RET_OK;
    }

    rmw_ret_t rmw_compare_gids_equal(const char *implementation_identifier, const rmw_gid_t *gid1, const rmw_gid_t *gid2, bool *result)
    {
      RMW_CHECK_ARGUMENT_FOR_NULL(gid1, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(gid2, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(result, RMW_RET_INVALID_ARGUMENT);
      CHECK_RMW_IMPLEMENTATION(implementation_identifier, gid1);

      *result = std::equal(std::begin(gid1->data), std::end(gid1->data), std::begin(gid2->data));

      return RMW_RET_OK;
    }

    rmw_ret_t rmw_service_server_is_available(const char *implementation_identifier,
                                              const rmw_node_t *node,
                                              const rmw_client_t *client,
                                              bool *is_available)
    {
      RMW_CHECK_ARGUMENT_FOR_NULL(node, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(client, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(is_available, RMW_RET_INVALID_ARGUMENT);
      CHECK_RMW_IMPLEMENTATION(implementation_identifier, node);

      auto ecal_client = GetImplementation(client);
      *is_available = ecal_client->IsServiceAvailable();

      return RMW_RET_OK;
    }

    rmw_ret_t rmw_set_log_severity(const char * /* implementation_identifier */, rmw_log_severity_t severity)
    {
      switch (severity)
      {
      case RMW_LOG_SEVERITY_DEBUG:
        eCAL::Logging::SetLogLevel(eCAL_Logging_eLogLevel::log_level_debug1);
        break;
      case RMW_LOG_SEVERITY_INFO:
        eCAL::Logging::SetLogLevel(eCAL_Logging_eLogLevel::log_level_info);
        break;
      case RMW_LOG_SEVERITY_WARN:
        eCAL::Logging::SetLogLevel(eCAL_Logging_eLogLevel::log_level_warning);
        break;
      case RMW_LOG_SEVERITY_ERROR:
        eCAL::Logging::SetLogLevel(eCAL_Logging_eLogLevel::log_level_error);
        break;
      case RMW_LOG_SEVERITY_FATAL:
        eCAL::Logging::SetLogLevel(eCAL_Logging_eLogLevel::log_level_fatal);
        break;
      }
      return RMW_RET_OK;
    }

    rmw_ret_t rmw_subscription_get_actual_qos(const char *implementation_identifier,
                                              const rmw_subscription_t *subscription,
                                              rmw_qos_profile_t *qos)
    {
      RMW_CHECK_ARGUMENT_FOR_NULL(subscription, RMW_RET_ERROR);
      RMW_CHECK_ARGUMENT_FOR_NULL(qos, RMW_RET_ERROR);
      CHECK_RMW_IMPLEMENTATION(implementation_identifier, subscription);

      auto ecal_sub = GetImplementation(subscription);
      *qos = ecal_sub->GetRosQOSProfile();

      return RMW_RET_OK;
    }

    rmw_ret_t rmw_init_publisher_allocation(const char * /* implementation_identifier */,
                                            const rosidl_message_type_support_t * /* type_support */,
                                            const rosidl_runtime_c__Sequence__bound * /* message_bounds */,
                                            rmw_publisher_allocation_t * /* allocation */)
    {
      UNSUPPORTED;
    }

    rmw_ret_t rmw_fini_publisher_allocation(const char * /* implementation_identifier */,
                                            rmw_publisher_allocation_t * /* allocation */)
    {
      UNSUPPORTED;
    }

    rmw_ret_t rmw_publisher_assert_liveliness(const char * /* implementation_identifier */,
                                              const rmw_publisher_t * /* publisher */)
    {
      UNSUPPORTED;
    }

    rmw_ret_t rmw_init_subscription_allocation(const char * /* implementation_identifier */,
                                               const rosidl_message_type_support_t * /* type_support */,
                                               const rosidl_runtime_c__Sequence__bound * /* message_bounds */,
                                               rmw_subscription_allocation_t * /* allocation */)
    {
      UNSUPPORTED;
    }

    rmw_ret_t rmw_fini_subscription_allocation(const char * /*implementation_identifier */,
                                               rmw_subscription_allocation_t * /* allocation */)
    {
      UNSUPPORTED;
    }

    rmw_ret_t rmw_take_loaned_message(const char * /* implementation_identifier */,
                                      const rmw_subscription_t * /* subscription */,
                                      void ** /* loaned_message */,
                                      bool * /* taken */,
                                      rmw_subscription_allocation_t * /* allocation */)
    {
      UNSUPPORTED;
    }

    rmw_ret_t rmw_take_loaned_message_with_info(const char * /* implementation_identifier */,
                                                const rmw_subscription_t * /* subscription */,
                                                void ** /* loaned_message */,
                                                bool * /* taken */,
                                                rmw_message_info_t * /* message_info */,
                                                rmw_subscription_allocation_t * /* allocation */)
    {
      UNSUPPORTED;
    }

    rmw_ret_t rmw_return_loaned_message_from_subscription(const char * /* implementation_identifier */,
                                                          const rmw_subscription_t * /* subscription */,
                                                          void * /* loaned_message */)
    {
      UNSUPPORTED;
    }

    rmw_ret_t rmw_borrow_loaned_message(const char * /* implementation_identifier */,
                                        const rmw_publisher_t * /* publisher */,
                                        const rosidl_message_type_support_t * /* type_support */,
                                        void ** /* ros_message */)
    {
      UNSUPPORTED;
    }

    rmw_ret_t rmw_publish_loaned_message(const char * /* implementation_identifier */,
                                         const rmw_publisher_t * /* publisher */,
                                         void * /* ros_message */,
                                         rmw_publisher_allocation_t * /* allocation */)
    {
      UNSUPPORTED;
    }

    rmw_ret_t rmw_return_loaned_message_from_publisher(const char * /* implementation_identifier */,
                                                       const rmw_publisher_t * /* publisher */,
                                                       void * /* loaned_message */)
    {
      UNSUPPORTED;
    }

    rmw_ret_t rmw_publisher_wait_for_all_acked(const rmw_publisher_t * /* publisher */,
                                               rmw_time_t /* wait_timeout */)
    {
      UNSUPPORTED;
    }

// required functions for HUMBLE
#if ROS_DISTRO == HUMBLE

    bool rmw_feature_supported(rmw_feature_t feature)
    {
        UNSUPPORTED; // ?
    }

    rmw_ret_t rmw_subscription_set_content_filter(rmw_subscription_t * subscription, const rmw_subscription_content_filter_options_t * options)
    {
      UNSUPPORTED; // OK
    }

    rmw_ret_t rmw_subscription_get_content_filter(const rmw_subscription_t * subscription, rcutils_allocator_t * allocator, rmw_subscription_content_filter_options_t * options)
    {
      UNSUPPORTED; // OK
    }

    rmw_ret_t rmw_subscription_get_actual_qos(const rmw_subscription_t * subscription, rmw_qos_profile_t * qos)
    {
      //TODO
      return RMW_RET_OK;
    }

    rmw_ret_t rmw_client_request_publisher_get_actual_qos(const rmw_client_t * client, rmw_qos_profile_t * qos)
    {
      // TODO
      return RMW_RET_OK;
    }

   rmw_ret_t rmw_client_response_subscription_get_actual_qos(const rmw_client_t * client, rmw_qos_profile_t * qos)
   {
      UNSUPPORTED; // TODO
   }

   rmw_ret_t rmw_client_set_on_new_response_callback(rmw_client_t * rmw_client, rmw_event_callback_t callback, const void * user_data)
   {
      UNSUPPORTED; // OK
   }

   rmw_ret_t rmw_service_set_on_new_request_callback(rmw_service_t * rmw_service, rmw_event_callback_t callback, const void * user_data)
   {
      UNSUPPORTED; // OK
   }

    rmw_ret_t rmw_subscription_set_on_new_message_callback(rmw_subscription_t * subscription, rmw_event_callback_t callback, const void * user_data)
    {
          UNSUPPORTED; // OK
    }


    rmw_ret_t rmw_event_set_callback(rmw_event_t * rmw_event, rmw_event_callback_t callback, const void * user_data)
    {
      UNSUPPORTED; // OK
    }

    rmw_ret_t rmw_service_request_subscription_get_actual_qos(const rmw_service_t * service, rmw_qos_profile_t * qos)
    {
      // TODO
      RMW_CHECK_ARGUMENT_FOR_NULL(service, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
        service,
        service->implementation_identifier,
        service->implementation_identifier,
        return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
      RMW_CHECK_ARGUMENT_FOR_NULL(qos, RMW_RET_INVALID_ARGUMENT);

      if (service->data == nullptr) {
        RMW_SET_ERROR_MSG("service info is null");
        return RMW_RET_ERROR;
      }


      return RMW_RET_OK;
    }

    rmw_ret_t rmw_service_response_publisher_get_actual_qos(const rmw_service_t * service, rmw_qos_profile_t * qos){
       // TODO

       return RMW_RET_OK;
    }

#endif


    } // namespace rmw
} // namespace eCAL
