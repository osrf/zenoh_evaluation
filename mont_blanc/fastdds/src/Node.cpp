// Copyright 2022 Open Source Robotics Foundation, Inc.
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

#include "Node.h"

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/publisher/qos/PublisherQos.hpp>
#include <fastdds/dds/publisher/qos/DataWriterQos.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>
#include <fastdds/rtps/transport/UDPv4TransportDescriptor.h>

#include <string>
#include <utility>
#include <map>
#include <vector>

using eprosima::fastdds::dds::DomainParticipantFactory;
using eprosima::fastdds::dds::DomainParticipantQos;
using eprosima::fastdds::dds::TypeSupport;

using eprosima::fastdds::dds::DataWriter;
using eprosima::fastdds::dds::DataReader;
using eprosima::fastdds::dds::PublicationMatchedStatus;

using eprosima::fastdds::dds::TOPIC_QOS_DEFAULT;
using eprosima::fastdds::dds::PUBLISHER_QOS_DEFAULT;
using eprosima::fastdds::dds::SUBSCRIBER_QOS_DEFAULT;
using eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT;
using eprosima::fastdds::dds::DATAWRITER_QOS_DEFAULT;

using eprosima::fastdds::dds::SubscriptionMatchedStatus;
using eprosima::fastdds::dds::Topic;

using eprosima::fastdds::rtps::UDPv4TransportDescriptor;

namespace montblanc
{

// DynamicDataWriterListener =======================================================================
void DynamicDataWriterListener::on_publication_matched(
  DataWriter *,
  const PublicationMatchedStatus & info) {}

// DynamicDataReaderListener =======================================================================
void DynamicDataReaderListener::on_subscription_matched(
  DataReader * reader, const SubscriptionMatchedStatus & info) {}

void DynamicDataReaderListener::on_data_available(DataReader * reader)
{
  next_sample_cb_(reader);
}

DynamicDataReaderListener::DynamicDataReaderListener(
  std::function<void(DataReader *)> next_sample_cb)
: next_sample_cb_(next_sample_cb) {}

// Node ============================================================================================
Node::Node(std::string name)
: name_(std::move(name)),
  participant_(nullptr),
  publisher_(nullptr),
  subscriber_(nullptr),
  topics_(std::map<std::string, Topic *>()),
  datawriters_(std::vector<DataWriter *>()),  // one Publisher controls multiple DataWriters
  datareaders_(std::vector<DataReader *>())  // one Subscriber manages multiple DataReaders
{}

Node::~Node()
{
  // Writers and Readers
  for (auto datawriter : datawriters_) {
    publisher_->delete_datawriter(datawriter);
  }
  for (auto datareader : datareaders_) {
    subscriber_->delete_datareader(datareader);
  }

  // Pubsub
  if (publisher_ != nullptr) {participant_->delete_publisher(publisher_);}
  if (subscriber_ != nullptr) {participant_->delete_subscriber(subscriber_);}

  // Topics
  for (auto topic_pair : topics_) {
    participant_->delete_topic(topic_pair.second);
  }

  DomainParticipantFactory::get_instance()->delete_participant(participant_);
}

bool Node::init()
{
  DomainParticipantQos pqos;
  pqos.name(name_.c_str()); 

  participant_ = DomainParticipantFactory::get_instance()->create_participant(0, pqos);
  if (participant_ == nullptr) {return false;}

  printf("[Node Participant Ready]: %s\n", name_.c_str());
  return true;
}

std::string Node::name() {return name_;}

DataWriter * Node::create_datawriter(
  std::string topic_name,
  TypeSupport type)
{
  type.register_type(participant_);

  // Pub
  if (publisher_ == nullptr) {
    publisher_ = participant_->create_publisher(PUBLISHER_QOS_DEFAULT, nullptr);
    assert(publisher_ != nullptr);
  }

  // Topic
  auto topic_it = topics_.find(topic_name);
  Topic * topic_;

  if (topic_it == topics_.end()) {
    topic_ = participant_->create_topic(
      topic_name.c_str(),
      type.get_type_name(),
      TOPIC_QOS_DEFAULT);
    assert(topic_ != nullptr);

    topics_.insert({topic_name, topic_});
  } else {
    topic_ = topic_it->second;
  }

  // Writer
  DynamicDataWriterListener * listener_ = new DynamicDataWriterListener();
  DataWriter * datawriter_ = publisher_->create_datawriter(
    topic_,
    DATAWRITER_QOS_DEFAULT,
    listener_);
  assert(datawriter_ != nullptr);

  datawriters_.push_back(datawriter_);

  return datawriter_;
}

DataReader * Node::create_datareader(
  std::string topic_name,
  TypeSupport type,
  std::function<void(DataReader *)> cb)
{
  type.register_type(participant_);

  // Sub
  if (subscriber_ == nullptr) {
    subscriber_ = participant_->create_subscriber(SUBSCRIBER_QOS_DEFAULT, nullptr);
    assert(subscriber_ != nullptr);
  }

  // Topic
  auto topic_it = topics_.find(topic_name);
  Topic * topic_;

  if (topic_it == topics_.end()) {
    topic_ = participant_->create_topic(
      topic_name.c_str(),
      type.get_type_name(),
      TOPIC_QOS_DEFAULT);
    assert(topic_ != nullptr);

    topics_.insert({topic_name, topic_});
  } else {
    topic_ = topic_it->second;
  }

  // Reader
  DynamicDataReaderListener * listener_ = new DynamicDataReaderListener(cb);
  DataReader * datareader_ = subscriber_->create_datareader(
    topic_,
    DATAREADER_QOS_DEFAULT,
    listener_);
  assert(datareader_ != nullptr);

  datareaders_.push_back(datareader_);

  return datareader_;
}

}  // namespace montblanc
