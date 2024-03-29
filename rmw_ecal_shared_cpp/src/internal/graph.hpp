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
#include <list>
#include <unordered_set>

#include <ecal/ecal.h>
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4127 4146 4800)
#endif
#include "ecal/monitoring.pb.h"
#include "subscriber.pb.h"
#include "publisher.pb.h"
#include "service.pb.h"
#include "client.pb.h"
#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include "internal/qos.hpp"
#include "internal/node.hpp"

namespace eCAL
{
	namespace rmw
	{
		namespace Graph
		{

			struct NodeInfo
			{
				NodeInfo(const std::string &_name, const std::string &_name_space)
					: name{_name}, name_space{_name_space}
				{
				}

				std::string name;
				std::string name_space;
			};

			struct TopicInfo
			{
				TopicInfo(const std::string &_name,
						  const std::string &_type)
					: name{_name}, type{_type}
				{
				}

				std::string name;
				std::string type;
			};

			struct ServiceInfo
			{
				ServiceInfo(const std::string &_name,
							const std::string &_request_type,
							const std::string &_response_type)
					: name{_name}, request_type{_request_type}, response_type{_response_type}
				{
				}

				std::string name;
				std::string request_type;
				std::string response_type;
			};

			enum class TopicEndpointType
			{
				PUBLISHER,
				SUBSCRIBER
			};

			enum class TopicEndpointQOSReliability
			{
				BEST_EFFORT,
				RELIABLE
			};

			enum class TopicEndpointQOSHistoryKind
			{
				KEEP_LAST,
				KEEP_ALL
			};

			struct TopicEndpointQOS
			{
				TopicEndpointQOSReliability reliability;
				TopicEndpointQOSHistoryKind history_kind;
				int32_t history_depth;
			};

			struct TopicEndpointInfo
			{
				TopicEndpointInfo(std::string node_name_,
						  std::string node_namespace_,
						  std::string type_,
						  TopicEndpointType endpoint_type_,
  				                  TopicEndpointQOS qos_profile_)
					: node_name{node_name_}, node_namespace{node_namespace_},
					type{type_}, endpoint_type{endpoint_type_},
					qos_profile{qos_profile_}
				{
				}

				std::string node_name;
				std::string node_namespace;
				std::string type;
				TopicEndpointType endpoint_type;
  				TopicEndpointQOS qos_profile;
			};

			namespace
			{

				inline eCAL::rmw::pb::Monitoring GetMonitoringSnapshot()
				{
					eCAL::rmw::pb::Monitoring monitoring;
					std::string monitoring_data;
					eCAL::Monitoring::GetMonitoring(monitoring_data);
					monitoring.ParseFromString(monitoring_data);

					return monitoring;
				}

			} // namespace

			inline Node *CreateNode(const std::string &name_space, const std::string &name)
			{
				return new Node(name_space, name);
			}

			inline void DestroyNode(Node *node)
			{
				delete node;
			}

			inline std::list<NodeInfo> GetNodes()
			{
				std::list<NodeInfo> node_names;

				auto monitoring = GetMonitoringSnapshot();
				auto &services = monitoring.services();

				auto prefix_size = node_query_service_prefix.size();
				for (auto &service : services)
				{
					auto &service_name = service.sname();
					if (service_name.rfind(node_query_service_prefix, 0) == 0)
					{
						auto name_begin_index = service_name.find_last_of("/");
						auto name = service_name.substr(name_begin_index + 1);
						auto name_space = service_name.substr(prefix_size, name_begin_index + 1 - prefix_size);
						node_names.emplace_back(name, name_space);
					}
				}

				return node_names;
			}

			inline std::list<TopicInfo> GetTopics()
			{
				std::list<TopicInfo> topics;

				auto monitoring = GetMonitoringSnapshot();
				auto &ecal_topics = monitoring.topics();

				std::unordered_set<std::string> already_processed_topics;

				for (auto &topic : ecal_topics)
				{
					if (already_processed_topics.find(topic.tname()) == already_processed_topics.end())
					{
						topics.emplace_back(topic.tname(), topic.ttype());
						already_processed_topics.emplace(topic.tname());
					}
				}

				return topics;
			}

			inline size_t CountSubscribers(const std::string &topic_name)
			{
				auto monitoring = GetMonitoringSnapshot();
				auto &topics = monitoring.topics();
				auto full_topic_name = pub_name_prefix + topic_name;

				return std::count_if(topics.begin(), topics.end(), [&full_topic_name](auto &topic) {
					return topic.tname() == full_topic_name && topic.direction() == "subscriber";
				});
			}

			inline size_t CountPublishers(const std::string &topic_name)
			{
				auto monitoring = GetMonitoringSnapshot();
				auto &topics = monitoring.topics();
				auto full_topic_name = pub_name_prefix + topic_name;

				return std::count_if(topics.begin(), topics.end(), [&full_topic_name](auto &topic) {
					return topic.tname() == full_topic_name && topic.direction() == "publisher";
				});
			}

			inline std::list<ServiceInfo> GetServices()
			{
				std::list<ServiceInfo> services;

				auto monitoring = GetMonitoringSnapshot();
				auto &ecal_services = monitoring.services();

				for (auto &service : ecal_services)
				{
					for (auto &method : service.methods())
					{
						services.emplace_back(service.sname() + "/" + method.mname(), method.req_type(), method.resp_type());
					}
				}

				return services;
			}

			inline pb::GraphInfo::Subscribers GetSubscribers(const std::string &node_namespace, const std::string &node_name)
			{
				pb::GraphInfo::Subscribers data;
				eCAL::ServiceResponseVecT response;

				auto service_name = BuildQueryServiceName(node_namespace, node_name);

				eCAL::CServiceClient client{service_name};

				client.Call("GetSubscribers", "", /* timeout */ -1, &response);

				data.ParseFromString(response[0].response);

				return data;
			}

			inline pb::GraphInfo::Publishers GetPublishers(const std::string &node_namespace, const std::string &node_name)
			{
				pb::GraphInfo::Publishers data;
				eCAL::ServiceResponseVecT response;

				auto service_name = BuildQueryServiceName(node_namespace, node_name);

				eCAL::CServiceClient client{service_name};

				client.Call("GetPublishers", "", /* timeout */ -1, &response);

				data.ParseFromString(response[0].response);

				return data;
			}

			inline pb::GraphInfo::Services GetServices(const std::string &node_namespace, const std::string &node_name)
			{
				pb::GraphInfo::Services data;
				eCAL::ServiceResponseVecT response;

				auto service_name = BuildQueryServiceName(node_namespace, node_name);

				eCAL::CServiceClient client{service_name};

				client.Call("GetServices", "", /* timeout */ -1, &response);

				data.ParseFromString(response[0].response);

				return data;
			}

			inline pb::GraphInfo::Clients GetClients(const std::string &node_namespace, const std::string &node_name)
			{
				pb::GraphInfo::Clients data;
				eCAL::ServiceResponseVecT response;

				auto service_name = BuildQueryServiceName(node_namespace, node_name);

				eCAL::CServiceClient client{service_name};

				client.Call("GetClients", "", /* timeout */ -1, &response);

				data.ParseFromString(response[0].response);

				return data;
			}

			inline std::list<TopicEndpointInfo> GetTopicEndpointInfo(const std::string &topic_name, TopicEndpointType endpoint_type)
			{
				std::list<TopicEndpointInfo> topics;

				auto monitoring = GetMonitoringSnapshot();
				auto &ecal_topics = monitoring.topics();

				for (auto &topic : ecal_topics)
				{
					if(topic.tname() == topic_name &&
					endpoint_type == (topic.direction() == "publisher" ? TopicEndpointType::PUBLISHER : TopicEndpointType::SUBSCRIBER))
					{
						auto ep_type = endpoint_type;
  						TopicEndpointQOS qos_profile;
						qos_profile.reliability =
							topic.tqos().reliability() == pb::QOS_eQOSPolicy_Reliability_best_effort_reliability_qos ?
							TopicEndpointQOSReliability::RELIABLE : TopicEndpointQOSReliability::BEST_EFFORT;
						qos_profile.history_kind = topic.tqos().history() == pb::QOS_eQOSPolicy_HistoryKind_keep_all_history_qos ?
							TopicEndpointQOSHistoryKind::KEEP_ALL : TopicEndpointQOSHistoryKind::KEEP_ALL;
						qos_profile.history_depth = topic.tqos().history_depth();
						auto get_attr = [&topic] (const std::string &name) -> std::string {
							auto search = topic.attr().find(name);
							if (search == topic.attr().end()) return std::string{""};
							return search->second;
						};
						topics.emplace_back(get_attr("node_name"), get_attr("node_namespace"), topic.ttype(), ep_type, qos_profile);
					}
				}

				return topics;
			}

		} // namespace Graph
	} // namespace rmw
} // namespace eCAL
