// Copyright 2020 Continental AG
//
// Licensed under the Apache License, Version 2.0 (const char *implementation_identifier, the "License");
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

#include <rmw/rmw.h>

#include <rmw_ecal_shared_cpp/typesupport_factory.hpp>
#include <rmw_ecal_shared_cpp/serializer_factory.hpp>
#include <rmw_ecal_shared_cpp/visibility.h>

namespace eCAL
{
  namespace rmw
  {
    RMW_PROTOBUF_SHARED_CPP_PUBLIC
#if ROS_DISTRO >= GALACTIC
    rmw_node_t *rmw_create_node(const char *implementation_identifier,
                                rmw_context_t *context,
                                const char *name,
                                const char *namespace_);
#else
    rmw_node_t *rmw_create_node(const char *implementation_identifier,
                                rmw_context_t *context,
                                const char *name,
                                const char *namespace_,
                                size_t domain_id,
                                bool localhost_only);
#endif

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_destroy_node(const char *implementation_identifier, rmw_node_t *node);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_node_assert_liveliness(const char *implementation_identifier, const rmw_node_t *node);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    const rmw_guard_condition_t *rmw_node_get_graph_guard_condition(const char *implementation_identifier, const rmw_node_t *node);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_publisher_t *rmw_create_publisher(const char *implementation_identifier,
                                          const TypesupportFactory &ecal_typesupport_factory,
                                          const rmw_node_t *node,
                                          const rosidl_message_type_support_t *type_support,
                                          const char *topic_name,
                                          const rmw_qos_profile_t *qos_policies,
                                          const rmw_publisher_options_t *publisher_options);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_destroy_publisher(const char *implementation_identifier, rmw_node_t *node, rmw_publisher_t *publisher);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_publish(const char *implementation_identifier, const rmw_publisher_t *publisher,
                          const void *ros_message,
                          rmw_publisher_allocation_t *allocation);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_publisher_count_matched_subscriptions(const char *implementation_identifier, const rmw_publisher_t *publisher,
                                                        size_t *subscription_count);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_publisher_get_actual_qos(const char *implementation_identifier, const rmw_publisher_t *publisher,
                                           rmw_qos_profile_t *qos);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_publish_serialized_message(const char *implementation_identifier,
                                             const rmw_publisher_t *publisher,
                                             const rmw_serialized_message_t *serialized_message,
                                             rmw_publisher_allocation_t *allocation);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_get_serialized_message_size(const char *implementation_identifier, const rosidl_message_type_support_t *type_support,
                                              const rosidl_runtime_c__Sequence__bound *message_bounds,
                                              size_t *size);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_serialize(const char *implementation_identifier,
                            const SerializerFactory &ecal_serializer_factory,
                            const void *ros_message,
                            const rosidl_message_type_support_t *type_support,
                            rmw_serialized_message_t *serialized_message);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_deserialize(const char *implementation_identifier,
                              const SerializerFactory &ecal_serializer_factory,
                              const rmw_serialized_message_t *serialized_message,
                              const rosidl_message_type_support_t *type_support,
                              void *ros_message);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_subscription_t *rmw_create_subscription(const char *implementation_identifier,
                                                const TypesupportFactory &ecal_typesupport_factory,
                                                const rmw_node_t *node,
                                                const rosidl_message_type_support_t *type_support,
                                                const char *topic_name,
                                                const rmw_qos_profile_t *qos_policies,
                                                const rmw_subscription_options_t *subscription_options);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_destroy_subscription(const char *implementation_identifier, rmw_node_t *node, rmw_subscription_t *subscription);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_subscription_count_matched_publishers(const char *implementation_identifier, const rmw_subscription_t *subscription,
                                                        size_t *publisher_count);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_take(const char *implementation_identifier, const rmw_subscription_t *subscription,
                       void *ros_message,
                       bool *taken,
                       rmw_subscription_allocation_t *allocation);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_take_with_info(const char *implementation_identifier, const rmw_subscription_t *subscription,
                                 void *ros_message,
                                 bool *taken,
                                 rmw_message_info_t *message_info,
                                 rmw_subscription_allocation_t *allocation);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_take_sequence(const char *implementation_identifier, const rmw_subscription_t *subscription,
                                size_t count,
                                rmw_message_sequence_t *message_sequence,
                                rmw_message_info_sequence_t *message_info_sequence,
                                size_t *taken,
                                rmw_subscription_allocation_t *allocation);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_take_serialized_message(const char *implementation_identifier, const rmw_subscription_t *subscription,
                                          rmw_serialized_message_t *serialized_message,
                                          bool *taken,
                                          rmw_subscription_allocation_t *allocation);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_take_serialized_message_with_info(const char *implementation_identifier, const rmw_subscription_t *subscription,
                                                    rmw_serialized_message_t *serialized_message,
                                                    bool *taken,
                                                    rmw_message_info_t *message_info,
                                                    rmw_subscription_allocation_t *allocation);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_client_t *rmw_create_client(const char *implementation_identifier,
                                    const TypesupportFactory &ecal_typesupport_factory,
                                    const rmw_node_t *node,
                                    const rosidl_service_type_support_t *type_support,
                                    const char *service_name,
                                    const rmw_qos_profile_t *qos_policies);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_destroy_client(const char *implementation_identifier, rmw_node_t *node, rmw_client_t *client);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_send_request(const char *implementation_identifier, const rmw_client_t *client,
                               const void *ros_request,
                               int64_t *sequence_id);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t
    rmw_take_response(const char *implementation_identifier, const rmw_client_t *client,
                      rmw_service_info_t *request_header,
                      void *ros_response,
                      bool *taken);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_service_t *rmw_create_service(const char *implementation_identifier,
                                      const TypesupportFactory &ecal_typesupport_factory,
                                      const rmw_node_t *node,
                                      const rosidl_service_type_support_t *type_support,
                                      const char *service_name,
                                      const rmw_qos_profile_t *qos_policies);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_destroy_service(const char *implementation_identifier, rmw_node_t *node, rmw_service_t *service);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_take_request(const char *implementation_identifier, const rmw_service_t *service,
                               rmw_service_info_t *request_header,
                               void *ros_request,
                               bool *taken);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_send_response(const char *implementation_identifier, const rmw_service_t *service,
                                rmw_request_id_t *request_header,
                                void *ros_response);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_guard_condition_t *rmw_create_guard_condition(const char *implementation_identifier, rmw_context_t *context);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_destroy_guard_condition(const char *implementation_identifier, rmw_guard_condition_t *guard_condition);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_trigger_guard_condition(const char *implementation_identifier, const rmw_guard_condition_t *guard_condition);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_wait_set_t *rmw_create_wait_set(const char *implementation_identifier, rmw_context_t *context, size_t max_conditions);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_destroy_wait_set(const char *implementation_identifier, rmw_wait_set_t *wait_set);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_wait(const char *implementation_identifier, rmw_subscriptions_t *subscriptions,
                       rmw_guard_conditions_t *guard_conditions,
                       rmw_services_t *services,
                       rmw_clients_t *clients,
                       rmw_events_t *events,
                       rmw_wait_set_t *wait_set,
                       const rmw_time_t *wait_timeout);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_get_node_names(const char *implementation_identifier, const rmw_node_t *node,
                                 rcutils_string_array_t *node_names,
                                 rcutils_string_array_t *node_namespaces);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_get_node_names_with_enclaves(const char *implementation_identifier, const rmw_node_t *node,
                                               rcutils_string_array_t *node_names,
                                               rcutils_string_array_t *node_namespaces,
                                               rcutils_string_array_t *enclaves);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_count_publishers(const char *implementation_identifier, const rmw_node_t *node,
                                   const char *topic_name,
                                   size_t *count);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_count_subscribers(const char *implementation_identifier, const rmw_node_t *node,
                                    const char *topic_name,
                                    size_t *count);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_get_gid_for_publisher(const char *implementation_identifier, const rmw_publisher_t *publisher, rmw_gid_t *gid);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_compare_gids_equal(const char *implementation_identifier, const rmw_gid_t *gid1, const rmw_gid_t *gid2, bool *result);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_service_server_is_available(const char *implementation_identifier, const rmw_node_t *node,
                                              const rmw_client_t *client,
                                              bool *is_available);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_set_log_severity(const char *implementation_identifier, rmw_log_severity_t severity);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_subscription_get_actual_qos(const char *implementation_identifier, const rmw_subscription_t *subscription,
                                              rmw_qos_profile_t *qos);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_init_publisher_allocation(const char *implementation_identifier, const rosidl_message_type_support_t *type_support,
                                            const rosidl_runtime_c__Sequence__bound *message_bounds,
                                            rmw_publisher_allocation_t *allocation);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_fini_publisher_allocation(const char *implementation_identifier, rmw_publisher_allocation_t *allocation);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_publisher_assert_liveliness(const char *implementation_identifier, const rmw_publisher_t *publisher);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_init_subscription_allocation(const char *implementation_identifier, const rosidl_message_type_support_t *type_support,
                                               const rosidl_runtime_c__Sequence__bound *message_bounds,
                                               rmw_subscription_allocation_t *allocation);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_fini_subscription_allocation(const char *implementation_identifier, rmw_subscription_allocation_t *allocation);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_take_loaned_message(const char *implementation_identifier, const rmw_subscription_t *subscription,
                                      void **loaned_message,
                                      bool *taken,
                                      rmw_subscription_allocation_t *allocation);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_take_loaned_message_with_info(const char *implementation_identifier, const rmw_subscription_t *subscription,
                                                void **loaned_message,
                                                bool *taken,
                                                rmw_message_info_t *message_info,
                                                rmw_subscription_allocation_t *allocation);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_return_loaned_message_from_subscription(const char *implementation_identifier, const rmw_subscription_t *subscription,
                                                          void *loaned_message);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_borrow_loaned_message(const char *implementation_identifier, const rmw_publisher_t *publisher,
                                        const rosidl_message_type_support_t *type_support,
                                        void **ros_message);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_publish_loaned_message(const char *implementation_identifier, const rmw_publisher_t *publisher,
                                         void *ros_message,
                                         rmw_publisher_allocation_t *allocation);

    RMW_PROTOBUF_SHARED_CPP_PUBLIC
    rmw_ret_t rmw_return_loaned_message_from_publisher(const char *implementation_identifier, const rmw_publisher_t *publisher,
                                                       void *loaned_message);
  } // namespace rmw
} // namespace eCAL
