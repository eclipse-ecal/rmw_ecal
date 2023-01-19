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

#include <rmw/rmw.h>

#include <rmw_ecal_shared_cpp/rmw/rmw.hpp>

#include "custom_typesupport_factory.hpp"
#include "custom_serializer_factory.hpp"

const auto identifier{"rmw_ecal_dynamic_cpp"};
const auto serialization_format{"custom"};

const char *rmw_get_implementation_identifier(void) {
    return identifier;
}

const char *rmw_get_serialization_format(void) {
    return serialization_format;
}

#if ROS_DISTRO >= GALACTIC

rmw_node_t *rmw_create_node(rmw_context_t *context,
                            const char *name,
                            const char *namespace_) {
    return eCAL::rmw::rmw_create_node(::rmw_get_implementation_identifier(), context, name, namespace_);
}

#else
rmw_node_t *rmw_create_node(rmw_context_t *context,
                            const char *name,
                            const char *namespace_,
                            size_t domain_id,
                            bool localhost_only)
{
  return eCAL::rmw::rmw_create_node(::rmw_get_implementation_identifier(), context, name, namespace_, domain_id, localhost_only);
}
#endif

rmw_ret_t rmw_destroy_node(rmw_node_t *node) {
    return eCAL::rmw::rmw_destroy_node(::rmw_get_implementation_identifier(), node);
}

rmw_ret_t rmw_node_assert_liveliness(const rmw_node_t *node) {
    return eCAL::rmw::rmw_node_assert_liveliness(::rmw_get_implementation_identifier(), node);
}

const rmw_guard_condition_t *rmw_node_get_graph_guard_condition(const rmw_node_t *node) {
    return eCAL::rmw::rmw_node_get_graph_guard_condition(::rmw_get_implementation_identifier(), node);
}

rmw_publisher_t *rmw_create_publisher(const rmw_node_t *node,
                                      const rosidl_message_type_support_t *type_support,
                                      const char *topic_name,
                                      const rmw_qos_profile_t *qos_policies,
                                      const rmw_publisher_options_t *publisher_options) {
    return eCAL::rmw::rmw_create_publisher(::rmw_get_implementation_identifier(), eCAL::rmw::CustomTypeSupportFactory{},
                                           node, type_support, topic_name, qos_policies, publisher_options);
}

rmw_ret_t rmw_destroy_publisher(rmw_node_t *node, rmw_publisher_t *publisher) {
    return eCAL::rmw::rmw_destroy_publisher(::rmw_get_implementation_identifier(), node, publisher);
}

rmw_ret_t rmw_publish(const rmw_publisher_t *publisher,
                      const void *ros_message,
                      rmw_publisher_allocation_t *allocation) {
    return eCAL::rmw::rmw_publish(::rmw_get_implementation_identifier(), publisher, ros_message, allocation);
}

rmw_ret_t rmw_publisher_count_matched_subscriptions(const rmw_publisher_t *publisher,
                                                    size_t *subscription_count) {
    return eCAL::rmw::rmw_publisher_count_matched_subscriptions(::rmw_get_implementation_identifier(), publisher,
                                                                subscription_count);
}

rmw_ret_t rmw_publisher_get_actual_qos(const rmw_publisher_t *publisher,
                                       rmw_qos_profile_t *qos) {
    return eCAL::rmw::rmw_publisher_get_actual_qos(::rmw_get_implementation_identifier(), publisher, qos);
}

rmw_ret_t rmw_publish_serialized_message(const rmw_publisher_t *publisher,
                                         const rmw_serialized_message_t *serialized_message,
                                         rmw_publisher_allocation_t *allocation) {
    return eCAL::rmw::rmw_publish_serialized_message(::rmw_get_implementation_identifier(), publisher,
                                                     serialized_message, allocation);
}

rmw_ret_t rmw_get_serialized_message_size(const rosidl_message_type_support_t *type_support,
                                          const rosidl_runtime_c__Sequence__bound *message_bounds,
                                          size_t *size) {
    return eCAL::rmw::rmw_get_serialized_message_size(::rmw_get_implementation_identifier(), type_support,
                                                      message_bounds, size);
}

rmw_ret_t rmw_serialize(const void *ros_message,
                        const rosidl_message_type_support_t *type_support,
                        rmw_serialized_message_t *serialized_message) {
    return eCAL::rmw::rmw_serialize(::rmw_get_implementation_identifier(), eCAL::rmw::CustomSerializerFactory{},
                                    ros_message, type_support, serialized_message);
}

rmw_ret_t rmw_deserialize(const rmw_serialized_message_t *serialized_message,
                          const rosidl_message_type_support_t *type_support,
                          void *ros_message) {
    return eCAL::rmw::rmw_deserialize(::rmw_get_implementation_identifier(), eCAL::rmw::CustomSerializerFactory{},
                                      serialized_message, type_support, ros_message);
}

rmw_subscription_t *rmw_create_subscription(const rmw_node_t *node,
                                            const rosidl_message_type_support_t *type_support,
                                            const char *topic_name,
                                            const rmw_qos_profile_t *qos_policies,
                                            const rmw_subscription_options_t *subscription_options) {
    return eCAL::rmw::rmw_create_subscription(::rmw_get_implementation_identifier(),
                                              eCAL::rmw::CustomTypeSupportFactory{}, node, type_support, topic_name,
                                              qos_policies, subscription_options);
}

rmw_ret_t rmw_destroy_subscription(rmw_node_t *node, rmw_subscription_t *subscription) {
    return eCAL::rmw::rmw_destroy_subscription(::rmw_get_implementation_identifier(), node, subscription);
}

rmw_ret_t rmw_subscription_count_matched_publishers(const rmw_subscription_t *subscription,
                                                    size_t *publisher_count) {
    return eCAL::rmw::rmw_subscription_count_matched_publishers(::rmw_get_implementation_identifier(), subscription,
                                                                publisher_count);
}

rmw_ret_t rmw_take(const rmw_subscription_t *subscription,
                   void *ros_message,
                   bool *taken,
                   rmw_subscription_allocation_t *allocation) {
    return eCAL::rmw::rmw_take(::rmw_get_implementation_identifier(), subscription, ros_message, taken, allocation);
}

rmw_ret_t rmw_take_with_info(const rmw_subscription_t *subscription,
                             void *ros_message,
                             bool *taken,
                             rmw_message_info_t *message_info,
                             rmw_subscription_allocation_t *allocation) {
    return eCAL::rmw::rmw_take_with_info(::rmw_get_implementation_identifier(), subscription, ros_message, taken,
                                         message_info, allocation);
}

rmw_ret_t rmw_take_sequence(const rmw_subscription_t *subscription,
                            size_t count,
                            rmw_message_sequence_t *message_sequence,
                            rmw_message_info_sequence_t *message_info_sequence,
                            size_t *taken,
                            rmw_subscription_allocation_t *allocation) {
    return eCAL::rmw::rmw_take_sequence(::rmw_get_implementation_identifier(), subscription, count, message_sequence,
                                        message_info_sequence,
                                        taken, allocation);
}

rmw_ret_t rmw_take_serialized_message(const rmw_subscription_t *subscription,
                                      rmw_serialized_message_t *serialized_message,
                                      bool *taken,
                                      rmw_subscription_allocation_t *allocation) {
    return eCAL::rmw::rmw_take_serialized_message(::rmw_get_implementation_identifier(), subscription,
                                                  serialized_message, taken, allocation);
}

rmw_ret_t rmw_take_serialized_message_with_info(const rmw_subscription_t *subscription,
                                                rmw_serialized_message_t *serialized_message,
                                                bool *taken,
                                                rmw_message_info_t *message_info,
                                                rmw_subscription_allocation_t *allocation) {
    return eCAL::rmw::rmw_take_serialized_message_with_info(::rmw_get_implementation_identifier(), subscription,
                                                            serialized_message, taken,
                                                            message_info, allocation);
}

rmw_client_t *rmw_create_client(const rmw_node_t *node,
                                const rosidl_service_type_support_t *type_support,
                                const char *service_name,
                                const rmw_qos_profile_t *qos_policies) {
    return eCAL::rmw::rmw_create_client(::rmw_get_implementation_identifier(), eCAL::rmw::CustomTypeSupportFactory{},
                                        node, type_support, service_name, qos_policies);
}

rmw_ret_t rmw_destroy_client(rmw_node_t *node, rmw_client_t *client) {
    return eCAL::rmw::rmw_destroy_client(::rmw_get_implementation_identifier(), node, client);
}

rmw_ret_t rmw_send_request(const rmw_client_t *client,
                           const void *ros_request,
                           int64_t *sequence_id) {
    return eCAL::rmw::rmw_send_request(::rmw_get_implementation_identifier(), client, ros_request, sequence_id);
}

rmw_ret_t rmw_take_response(const rmw_client_t *client,
                            rmw_service_info_t *request_header,
                            void *ros_response,
                            bool *taken) {
    return eCAL::rmw::rmw_take_response(::rmw_get_implementation_identifier(), client, request_header, ros_response,
                                        taken);
}

rmw_service_t *rmw_create_service(const rmw_node_t *node,
                                  const rosidl_service_type_support_t *type_support,
                                  const char *service_name,
                                  const rmw_qos_profile_t *qos_policies) {
    return eCAL::rmw::rmw_create_service(::rmw_get_implementation_identifier(), eCAL::rmw::CustomTypeSupportFactory{},
                                         node, type_support, service_name, qos_policies);
}

rmw_ret_t rmw_destroy_service(rmw_node_t *node, rmw_service_t *service) {
    return eCAL::rmw::rmw_destroy_service(::rmw_get_implementation_identifier(), node, service);
}

rmw_ret_t rmw_take_request(const rmw_service_t *service,
                           rmw_service_info_t *request_header,
                           void *ros_request,
                           bool *taken) {
    return eCAL::rmw::rmw_take_request(::rmw_get_implementation_identifier(), service, request_header, ros_request,
                                       taken);
}

rmw_ret_t rmw_send_response(const rmw_service_t *service,
                            rmw_request_id_t *request_header,
                            void *ros_response) {
    return eCAL::rmw::rmw_send_response(::rmw_get_implementation_identifier(), service, request_header, ros_response);
}

rmw_guard_condition_t *rmw_create_guard_condition(rmw_context_t *context) {
    return eCAL::rmw::rmw_create_guard_condition(::rmw_get_implementation_identifier(), context);
}

rmw_ret_t rmw_destroy_guard_condition(rmw_guard_condition_t *guard_condition) {
    return eCAL::rmw::rmw_destroy_guard_condition(::rmw_get_implementation_identifier(), guard_condition);
}

rmw_ret_t rmw_trigger_guard_condition(const rmw_guard_condition_t *guard_condition) {
    return eCAL::rmw::rmw_trigger_guard_condition(::rmw_get_implementation_identifier(), guard_condition);
}

rmw_wait_set_t *rmw_create_wait_set(rmw_context_t *context, size_t max_conditions) {
    return eCAL::rmw::rmw_create_wait_set(::rmw_get_implementation_identifier(), context, max_conditions);
}

rmw_ret_t rmw_destroy_wait_set(rmw_wait_set_t *wait_set) {
    return eCAL::rmw::rmw_destroy_wait_set(::rmw_get_implementation_identifier(), wait_set);
}

rmw_ret_t rmw_wait(rmw_subscriptions_t *subscriptions,
                   rmw_guard_conditions_t *guard_conditions,
                   rmw_services_t *services,
                   rmw_clients_t *clients,
                   rmw_events_t *events,
                   rmw_wait_set_t *wait_set,
                   const rmw_time_t *wait_timeout) {
    return eCAL::rmw::rmw_wait(::rmw_get_implementation_identifier(), subscriptions, guard_conditions, services,
                               clients,
                               events, wait_set, wait_timeout);
}

rmw_ret_t rmw_get_node_names(const rmw_node_t *node,
                             rcutils_string_array_t *node_names,
                             rcutils_string_array_t *node_namespaces) {
    return eCAL::rmw::rmw_get_node_names(::rmw_get_implementation_identifier(), node, node_names, node_namespaces);
}

rmw_ret_t rmw_get_node_names_with_enclaves(const rmw_node_t *node,
                                           rcutils_string_array_t *node_names,
                                           rcutils_string_array_t *node_namespaces,
                                           rcutils_string_array_t *enclaves) {
    return eCAL::rmw::rmw_get_node_names_with_enclaves(::rmw_get_implementation_identifier(), node, node_names,
                                                       node_namespaces, enclaves);
}

rmw_ret_t rmw_count_publishers(const rmw_node_t *node,
                               const char *topic_name,
                               size_t *count) {
    return eCAL::rmw::rmw_count_publishers(::rmw_get_implementation_identifier(), node, topic_name, count);
}

rmw_ret_t rmw_count_subscribers(const rmw_node_t *node,
                                const char *topic_name,
                                size_t *count) {
    return eCAL::rmw::rmw_count_subscribers(::rmw_get_implementation_identifier(), node, topic_name, count);
}

rmw_ret_t rmw_get_gid_for_publisher(const rmw_publisher_t *publisher, rmw_gid_t *gid) {
    return eCAL::rmw::rmw_get_gid_for_publisher(::rmw_get_implementation_identifier(), publisher, gid);
}

rmw_ret_t rmw_compare_gids_equal(const rmw_gid_t *gid1, const rmw_gid_t *gid2, bool *result) {
    return eCAL::rmw::rmw_compare_gids_equal(::rmw_get_implementation_identifier(), gid1, gid2, result);
}

rmw_ret_t rmw_service_server_is_available(const rmw_node_t *node,
                                          const rmw_client_t *client,
                                          bool *is_available) {
    return eCAL::rmw::rmw_service_server_is_available(::rmw_get_implementation_identifier(), node, client,
                                                      is_available);
}

rmw_ret_t rmw_set_log_severity(rmw_log_severity_t severity) {
    return eCAL::rmw::rmw_set_log_severity(::rmw_get_implementation_identifier(), severity);
}

rmw_ret_t rmw_subscription_get_actual_qos(const rmw_subscription_t *subscription,
                                          rmw_qos_profile_t *qos) {
    return eCAL::rmw::rmw_subscription_get_actual_qos(::rmw_get_implementation_identifier(), subscription, qos);
}

rmw_ret_t rmw_init_publisher_allocation(const rosidl_message_type_support_t *type_support,
                                        const rosidl_runtime_c__Sequence__bound *message_bounds,
                                        rmw_publisher_allocation_t *allocation) {
    return eCAL::rmw::rmw_init_publisher_allocation(::rmw_get_implementation_identifier(), type_support, message_bounds,
                                                    allocation);
}

rmw_ret_t rmw_fini_publisher_allocation(rmw_publisher_allocation_t *allocation) {
    return eCAL::rmw::rmw_fini_publisher_allocation(::rmw_get_implementation_identifier(), allocation);
}

rmw_ret_t rmw_publisher_assert_liveliness(const rmw_publisher_t *publisher) {
    return eCAL::rmw::rmw_publisher_assert_liveliness(::rmw_get_implementation_identifier(), publisher);
}

rmw_ret_t rmw_init_subscription_allocation(const rosidl_message_type_support_t *type_support,
                                           const rosidl_runtime_c__Sequence__bound *message_bounds,
                                           rmw_subscription_allocation_t *allocation) {
    return eCAL::rmw::rmw_init_subscription_allocation(::rmw_get_implementation_identifier(), type_support,
                                                       message_bounds, allocation);
}

rmw_ret_t rmw_fini_subscription_allocation(rmw_subscription_allocation_t *allocation) {
    return eCAL::rmw::rmw_fini_subscription_allocation(::rmw_get_implementation_identifier(), allocation);
}

rmw_ret_t rmw_take_loaned_message(const rmw_subscription_t *subscription,
                                  void **loaned_message,
                                  bool *taken,
                                  rmw_subscription_allocation_t *allocation) {
    return eCAL::rmw::rmw_take_loaned_message(::rmw_get_implementation_identifier(), subscription, loaned_message,
                                              taken, allocation);
}

rmw_ret_t rmw_take_loaned_message_with_info(const rmw_subscription_t *subscription,
                                            void **loaned_message,
                                            bool *taken,
                                            rmw_message_info_t *message_info,
                                            rmw_subscription_allocation_t *allocation) {
    return eCAL::rmw::rmw_take_loaned_message_with_info(::rmw_get_implementation_identifier(), subscription,
                                                        loaned_message, taken, message_info, allocation);
}

rmw_ret_t rmw_return_loaned_message_from_subscription(const rmw_subscription_t *subscription,
                                                      void *loaned_message) {
    return eCAL::rmw::rmw_return_loaned_message_from_subscription(::rmw_get_implementation_identifier(), subscription,
                                                                  loaned_message);
}

rmw_ret_t rmw_borrow_loaned_message(const rmw_publisher_t *publisher,
                                    const rosidl_message_type_support_t *type_support,
                                    void **ros_message) {
    return eCAL::rmw::rmw_borrow_loaned_message(::rmw_get_implementation_identifier(), publisher, type_support,
                                                ros_message);
}

rmw_ret_t rmw_publish_loaned_message(const rmw_publisher_t *publisher,
                                     void *ros_message,
                                     rmw_publisher_allocation_t *allocation) {
    return eCAL::rmw::rmw_publish_loaned_message(::rmw_get_implementation_identifier(), publisher, ros_message,
                                                 allocation);
}

rmw_ret_t rmw_return_loaned_message_from_publisher(const rmw_publisher_t *publisher,
                                                   void *loaned_message) {
    return eCAL::rmw::rmw_return_loaned_message_from_publisher(::rmw_get_implementation_identifier(), publisher,
                                                               loaned_message);
}

rmw_ret_t rmw_publisher_wait_for_all_acked(const rmw_publisher_t *publisher, rmw_time_t wait_timeout) {
    return eCAL::rmw::rmw_publisher_wait_for_all_acked(publisher, wait_timeout);
}



// required functions for HUMBLE
#if ROS_DISTRO == HUMBLE

bool rmw_feature_supported(rmw_feature_t feature) {
    return eCAL::rmw::rmw_feature_supported(feature);
}

rmw_ret_t rmw_subscription_set_content_filter(rmw_subscription_t *subscription,
                                              const rmw_subscription_content_filter_options_t *options) {
    return eCAL::rmw::rmw_subscription_set_content_filter(subscription, options);
}

rmw_ret_t rmw_subscription_get_content_filter(const rmw_subscription_t *subscription, rcutils_allocator_t *allocator,
                                              rmw_subscription_content_filter_options_t *options) {
    return eCAL::rmw::rmw_subscription_get_content_filter(subscription, allocator, options);
}


rmw_ret_t rmw_client_request_publisher_get_actual_qos(const rmw_client_t *client, rmw_qos_profile_t *qos) {
    return eCAL::rmw::rmw_client_request_publisher_get_actual_qos(client, qos);
}


rmw_ret_t rmw_client_response_subscription_get_actual_qos(const rmw_client_t *client, rmw_qos_profile_t *qos) {
    return eCAL::rmw::rmw_client_response_subscription_get_actual_qos(client, qos);
}


rmw_ret_t rmw_client_set_on_new_response_callback(rmw_client_t *rmw_client, rmw_event_callback_t callback,
                                                  const void *user_data) {
    return eCAL::rmw::rmw_client_set_on_new_response_callback(rmw_client, callback, user_data);
}


rmw_ret_t rmw_service_set_on_new_request_callback(rmw_service_t *rmw_service, rmw_event_callback_t callback,
                                                  const void *user_data) {
    return eCAL::rmw::rmw_service_set_on_new_request_callback(rmw_service, callback, user_data);
}


rmw_ret_t rmw_subscription_set_on_new_message_callback(rmw_subscription_t *subscription, rmw_event_callback_t callback,
                                                       const void *user_data) {
    return eCAL::rmw::rmw_subscription_set_on_new_message_callback(subscription, callback, user_data);
}


rmw_ret_t rmw_event_set_callback(rmw_event_t *rmw_event, rmw_event_callback_t callback, const void *user_data) {
    return eCAL::rmw::rmw_event_set_callback(rmw_event, callback, user_data);
}

rmw_ret_t rmw_service_request_subscription_get_actual_qos(const rmw_service_t *service, rmw_qos_profile_t *qos) {
    return eCAL::rmw::rmw_service_request_subscription_get_actual_qos(service, qos);
}

rmw_ret_t rmw_service_response_publisher_get_actual_qos(const rmw_service_t *service, rmw_qos_profile_t *qos) {
    return eCAL::rmw::rmw_service_response_publisher_get_actual_qos(service, qos);
}

#endif
