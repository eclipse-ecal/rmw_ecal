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

#if ROS_DISTRO >= FOXY

#include <algorithm>

#include "rmw_ecal_shared_cpp/rmw/get_topic_endpoint_info.hpp"

#include "internal/graph.hpp"

namespace eCAL
{
  namespace rmw
  {

    rmw_endpoint_type_t to_ros_endpoint_type(eCAL::rmw::Graph::TopicEndpointType endpoint_type)
    {
      return endpoint_type == eCAL::rmw::Graph::TopicEndpointType::PUBLISHER ? RMW_ENDPOINT_PUBLISHER : RMW_ENDPOINT_SUBSCRIPTION;
    }

    rmw_qos_history_policy_t to_ros_policy(eCAL::rmw::Graph::TopicEndpointQOSHistoryKind history_policy)
    {
      switch (history_policy)
      {
      case eCAL::rmw::Graph::TopicEndpointQOSHistoryKind::KEEP_LAST:
        return RMW_QOS_POLICY_HISTORY_KEEP_LAST;
      case eCAL::rmw::Graph::TopicEndpointQOSHistoryKind::KEEP_ALL:
        return  RMW_QOS_POLICY_HISTORY_KEEP_ALL;
      default:
        throw std::invalid_argument{"Invalid history policy value."};
      }
    }

    rmw_qos_reliability_policy_t to_ros_policy(eCAL::rmw::Graph::TopicEndpointQOSReliability reliability_policy)
    {
      switch (reliability_policy)
      {
      case eCAL::rmw::Graph::TopicEndpointQOSReliability::BEST_EFFORT:
        return RMW_QOS_POLICY_RELIABILITY_BEST_EFFORT;
      case eCAL::rmw::Graph::TopicEndpointQOSReliability::RELIABLE:
        return RMW_QOS_POLICY_RELIABILITY_RELIABLE;
      default:
        throw std::invalid_argument{"Invalid reliability policy value."};
      }
    }

    rmw_qos_profile_t to_ros_qos_profile(eCAL::rmw::Graph::TopicEndpointQOS endpoint_qos)
    {
      auto qos_profile = rmw_qos_profile_t{};
      qos_profile.avoid_ros_namespace_conventions = false;
      qos_profile.history = to_ros_policy(endpoint_qos.history_kind);
      qos_profile.depth = endpoint_qos.history_depth;
      qos_profile.reliability = to_ros_policy(endpoint_qos.reliability);
      qos_profile.deadline = {0, 0};
      qos_profile.durability = RMW_QOS_POLICY_DURABILITY_TRANSIENT_LOCAL;
      qos_profile.lifespan = {0, 0};
      qos_profile.liveliness = RMW_QOS_POLICY_LIVELINESS_UNKNOWN;
      qos_profile.liveliness_lease_duration = {0, 0};
      return qos_profile;
    }

    void transform_to_rmw_topic_endpoint_info_array(std::list<eCAL::rmw::Graph::TopicEndpointInfo>& topic_info, rmw_topic_endpoint_info_array_t *rmw_topic_info, rcutils_allocator_t *allocator)
    {
      auto init_success = rmw_topic_endpoint_info_array_init_with_size(rmw_topic_info, topic_info.size(), allocator);
      if (init_success != RMW_RET_OK)
      {
        throw std::runtime_error("Failed to initialize rmw_topic_endpoint_info_array_t.");
      }
      std::transform(topic_info.begin(), topic_info.end(), RosArray::Begin(*rmw_topic_info),
      [allocator](auto &topic) {
        auto ep_info = rmw_get_zero_initialized_topic_endpoint_info();
        auto result = rmw_topic_endpoint_info_set_topic_type(&ep_info, topic.type.c_str(), allocator);
        if (result != RMW_RET_OK)
        {
          throw std::runtime_error("Failed to set topic type");
        }
        result = rmw_topic_endpoint_info_set_node_name(&ep_info, topic.node_name.c_str(), allocator);
        if (result != RMW_RET_OK)
        {
          throw std::runtime_error("Failed to set node name");
        }
        result = rmw_topic_endpoint_info_set_node_namespace(&ep_info, topic.node_namespace.c_str(), allocator);
        if (result != RMW_RET_OK)
        {
         throw std::runtime_error("Failed to set node namespace");
        }
        result = rmw_topic_endpoint_info_set_endpoint_type(&ep_info, to_ros_endpoint_type(topic.endpoint_type));
        if (result != RMW_RET_OK)
        {
          throw std::runtime_error("Failed to set endpoint type");
        }
        std::fill(std::begin(ep_info.endpoint_gid), std::end(ep_info.endpoint_gid), uint8_t{0});
        auto qos_profile = to_ros_qos_profile(topic.qos_profile);
        result = rmw_topic_endpoint_info_set_qos_profile(&ep_info, &qos_profile);
        if (result != RMW_RET_OK)
        {
          throw std::runtime_error("Failed to set qos");
        }
        return ep_info;
      });
    }

    rmw_ret_t rmw_get_publishers_info_by_topic(const char *implementation_identifier,
                                               const rmw_node_t *node,
                                               rcutils_allocator_t *allocator,
                                               const char *topic_name,
                                               bool no_mangle,
                                               rmw_topic_endpoint_info_array_t *publishers_info)
    {
      CHECK_RMW_IMPLEMENTATION(implementation_identifier, node);
      RMW_CHECK_ARGUMENT_FOR_NULL(topic_name, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(publishers_info, RMW_RET_INVALID_ARGUMENT);

      auto mangled_topic_name = no_mangle ? topic_name : eCAL::rmw::MangleTopicName(topic_name);
      auto topic_info = eCAL::rmw::Graph::GetTopicEndpointInfo(mangled_topic_name, eCAL::rmw::Graph::TopicEndpointType::PUBLISHER);

      try
      {
	transform_to_rmw_topic_endpoint_info_array(topic_info, publishers_info, allocator);
      }
      catch (const std::runtime_error &e)
      {
        RMW_SET_ERROR_MSG(e.what());
        return RMW_RET_ERROR;
      }

      return RMW_RET_OK;
    }

    rmw_ret_t rmw_get_subscriptions_info_by_topic(const char *implementation_identifier,
                                                  const rmw_node_t *node,
                                                  rcutils_allocator_t *allocator,
                                                  const char *topic_name,
                                                  bool no_mangle,
                                                  rmw_topic_endpoint_info_array_t *subscriptions_info)
    {
      CHECK_RMW_IMPLEMENTATION(implementation_identifier, node);
      RMW_CHECK_ARGUMENT_FOR_NULL(topic_name, RMW_RET_INVALID_ARGUMENT);
      RMW_CHECK_ARGUMENT_FOR_NULL(subscriptions_info, RMW_RET_INVALID_ARGUMENT);

      auto mangled_topic_name = no_mangle ? topic_name : eCAL::rmw::MangleTopicName(topic_name);
      auto topic_info = eCAL::rmw::Graph::GetTopicEndpointInfo(mangled_topic_name, eCAL::rmw::Graph::TopicEndpointType::SUBSCRIBER);

      try
      {
	transform_to_rmw_topic_endpoint_info_array(topic_info, subscriptions_info, allocator);
      }
      catch (const std::runtime_error &e)
      {
        RMW_SET_ERROR_MSG(e.what());
        return RMW_RET_ERROR;
      }

      std::cout << "ret" << std::endl;
      return RMW_RET_OK;
    }

  } // namespace rmw
} // namespace eCAL
#endif
