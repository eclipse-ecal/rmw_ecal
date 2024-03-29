/* ========================= RMW eCAL LICENSE =================================
 *
 * Copyright (C) 2019 - 2020 Continental Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * ========================= RMW eCAL LICENSE =================================
*/

#pragma once

#include <string>
#include <mutex>
#include <queue>
#include <functional>
#include <atomic>
#include <memory>
#include <algorithm>

#include <ecal/ecal.h>

#include "rmw_ecal_shared_cpp/service_typesupport.hpp"

#include "internal/qos.hpp"
#include "internal/common.hpp"
#include "internal/wait_set.hpp"

namespace eCAL
{
namespace rmw
{

class Client
{
	struct Response
	{
		Response(sequence_number_t sequence_number,
				 char *data,
				 size_t data_size) : sequence_number(sequence_number),
									 data(data),
									 data_size(data_size)
		{
		}
		sequence_number_t sequence_number;
		char *data;
		size_t data_size;
	};

	struct Request
	{
		Request(sequence_number_t sequence_number,
				std::string &&data) : sequence_number(sequence_number),
									  data(std::move(data))
		{
		}
		sequence_number_t sequence_number;
		std::string data;
	};

	std::string name_;
	std::unique_ptr<ServiceTypeSupport> type_support_;
	eCAL::CServiceClient client_;
	WaitSet *wait_set_ = nullptr;
	mutable std::mutex wait_set_mutex_;
	mutable std::mutex response_queue_mutex_;
	mutable std::mutex request_queue_mutex_;
	std::queue<Response> responses_;
	std::queue<Request> requests_;

	void EnqueueResponse(const std::string &response)
	{
		auto actual_data_size = GetSequenceDataSize(response);
		auto seq_no = GetSequenceNumber(response);
		auto data = new char[actual_data_size];
		std::copy_n(response.data(), actual_data_size, data);

		std::lock_guard<std::mutex> queue_lock(response_queue_mutex_);
		responses_.emplace(seq_no, data, actual_data_size);
	}

	void OnResponse(const SServiceResponse &response)
	{
		EnqueueResponse(response.response);
		NotifyWaitSet();
		PopLastRequest();
		PerformNextRequest();
	}

	void NotifyWaitSet()
	{
		auto wait_set = GetAttachedWaitSet();
		if (wait_set)
		{
			std::lock_guard<std::mutex> ws_condition_lock(wait_set->condition_mutex);
			wait_set->Trigger();
		}
	}

	WaitSet *GetAttachedWaitSet() const
	{
		std::lock_guard<std::mutex> ws_lock(wait_set_mutex_);
		return wait_set_;
	}

	Response PopResponse()
	{
		std::lock_guard<std::mutex> lock(response_queue_mutex_);
		auto latest_data = responses_.front();
		responses_.pop();

		return latest_data;
	}

	sequence_number_t EnqueueRequest(const void *data)
	{
		auto sequence_number = GenerateSequenceNumber();
		auto sequence_number_bytes = reinterpret_cast<char *>(&sequence_number);
		auto serialized_data = type_support_->SerializeRequest(data);
		serialized_data.append(sequence_number_bytes, sizeof(sequence_number_t));

		std::lock_guard<std::mutex> queue_lock(request_queue_mutex_);
		requests_.emplace(sequence_number, std::move(serialized_data));

		if(requests_.size() == 1)
		{
			auto &latest_data = requests_.front();
			client_.CallAsync(type_support_->GetServiceSimpleName(), latest_data.data);
		}

		return sequence_number;
	}

	void PerformNextRequest()
	{
		std::lock_guard<std::mutex> queue_lock(request_queue_mutex_);
		if(!requests_.empty())
		{
			auto &latest_data = requests_.front();
			client_.CallAsync(type_support_->GetServiceSimpleName(), latest_data.data);
		}
	}

	void PopLastRequest()
	{
		std::lock_guard<std::mutex> queue_lock(request_queue_mutex_);
		requests_.pop();
	}

public:
	Client(const std::string &name, ServiceTypeSupport *type_support, const ClientQOS &qos) : name_(name),
																							  type_support_(type_support),
																							  client_(qos.service_name_prefix + name)
	{
		using namespace std::placeholders;

		client_.AddResponseCallback(std::bind(&Client::OnResponse, this, _1));
	}

	sequence_number_t SendRequest(const void *data)
	{	
		return EnqueueRequest(data);
	}

	sequence_number_t TakeResponse(void *data)
	{
		auto resp = PopResponse();
		type_support_->DeserializeResponse(data, resp.data, resp.data_size);
		delete[] resp.data;
		return resp.sequence_number;
	}

	bool HasResponse() const
	{
		std::lock_guard<std::mutex> lock(response_queue_mutex_);
		return !responses_.empty();
	}

	std::string GetName() const
	{
		return name_ + "/" + type_support_->GetServiceSimpleName();
	}

	std::string GetRequestType() const
	{
		return type_support_->GetRequestMessageName();
	}

	std::string GetResponseType() const
	{
		return type_support_->GetResponseMessageName();
	}

	void AttachWaitSet(WaitSet *wait_set)
	{
		std::lock_guard<std::mutex> lock(wait_set_mutex_);
		wait_set_ = wait_set;
	}

	void DetachWaitSet()
	{
		std::lock_guard<std::mutex> lock(wait_set_mutex_);
		wait_set_ = nullptr;
	}

	bool IsServiceAvailable()
	{
		eCAL::ServiceResponseVecT response;

		return client_.Call("_Ping" + type_support_->GetServiceSimpleName(), "", -1, &response);
	}
};

} // namespace rmw
} // namespace eCAL
