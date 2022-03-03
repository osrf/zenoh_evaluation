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

#ifndef NODE_H_
#define NODE_H_

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>

#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>

#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>

#include <iostream>
#include <string>
#include <functional>
#include <map>
#include <vector>

namespace montblanc
{

// DynamicDataWriterListener =======================================================================
class DynamicDataWriterListener : public eprosima::fastdds::dds::DataWriterListener
{
public:
  void on_publication_matched(
    eprosima::fastdds::dds::DataWriter* writer,
    const eprosima::fastdds::dds::PublicationMatchedStatus& info) override;
};


// DynamicDataReaderListener =======================================================================
class DynamicDataReaderListener : public eprosima::fastdds::dds::DataReaderListener
{
public:
  DynamicDataReaderListener(
    std::function<void(eprosima::fastdds::dds::DataReader *)> next_sample_cb_);

  void on_data_available(eprosima::fastdds::dds::DataReader* reader) override;
  void on_subscription_matched(
    eprosima::fastdds::dds::DataReader* reader,
    const eprosima::fastdds::dds::SubscriptionMatchedStatus& info) override;

private:
   std::function<void(eprosima::fastdds::dds::DataReader *)> next_sample_cb_;
};


// Node ============================================================================================
class Node
{
public:
  explicit Node(std::string name);
  ~Node();

  bool init();
  std::string name();

  eprosima::fastdds::dds::DataWriter* create_datawriter(
    std::string topic_name,
    eprosima::fastdds::dds::TypeSupport type);
  eprosima::fastdds::dds::DataReader* create_datareader(
    std::string topic_name,
    eprosima::fastdds::dds::TypeSupport type,
    std::function<void(eprosima::fastdds::dds::DataReader *)> cb);

private:
  eprosima::fastdds::dds::DomainParticipant* participant_;

  eprosima::fastdds::dds::Publisher* publisher_;
  eprosima::fastdds::dds::Subscriber* subscriber_;

  std::map<std::string, eprosima::fastdds::dds::Topic*> topics_;

  std::vector<eprosima::fastdds::dds::DataWriter*> datawriters_;
  std::vector<eprosima::fastdds::dds::DataReader*> datareaders_;

  std::string name_;
};

}  // namespace montblanc

#endif  // NODE_H_
