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

#include <iomanip>
#include <iostream>
#include <chrono>
#include <thread>
#include <string>

#include "Node.h"
#include "utils.hpp"

#include "types/datatypes.h"
#include "types/datatypesPubSubTypes.h"

#include <fastdds/dds/log/StdoutConsumer.hpp>

using namespace eprosima::fastdds::dds;

int main()
{
  std::string name = "Subscriber";

  std::unique_ptr<StdoutConsumer> stdout_consumer(new StdoutConsumer());
  Log::RegisterConsumer(std::move(stdout_consumer));
  Log::SetVerbosity(Log::Kind::Info);

  dds_node::Node node = dds_node::Node(name.c_str());
  node.init();

  DataReader * reader = node.create_datareader(
    "/amazon",
    static_cast<TypeSupport>(new BigDataPubSubType()),
    [&](DataReader * reader) -> void
    {
      BigData msg;
      SampleInfo info;

      if (reader->take_next_sample(&msg, &info) == ReturnCode_t::RETCODE_OK) {
        if (info.valid_data) {
          auto transmission_finish_time = std::chrono::high_resolution_clock::now();
          auto transmission_start_time =
              std::chrono::seconds(msg.timestamp().sec()) +
              std::chrono::nanoseconds(msg.timestamp().nanosec());

          auto transmission_time = transmission_finish_time - transmission_start_time;

          int64_t transmission_size = msg.getCdrSerializedSize(msg);
          std::chrono::duration<double> elapsed = transmission_time.time_since_epoch();
          double rate = transmission_size / elapsed.count();
          double bandwidth_bps = rate * 8.0;
          double bandwidth_mbps = bandwidth_bps / 1024.0 / 1024.0;

          std::cout <<
            std::chrono::duration_cast<std::chrono::seconds>(
              transmission_finish_time.time_since_epoch()).count() <<
            '.' << std::setfill('0') << std::setw(9) <<
            std::chrono::duration_cast<std::chrono::nanoseconds>(
              transmission_finish_time.time_since_epoch()).count() % 1'000'000'000 <<
            ',' << transmission_time.time_since_epoch().count() <<
            ',' << transmission_size <<
            ',' << std::fixed << rate <<
            ',' << std::fixed << bandwidth_bps <<
            ',' << std::fixed << bandwidth_mbps <<
            '\n';
        } else {
          std::cout << name << ": Sample data is invalid\n";
        }
      } else {
        std::cout << name << ": Failed to take sample\n";
      }
    }
  );

  std::cout << "Received at,Transmission time (s),Transmitted (Bytes),Rate (B/s),Bandwidth (bps),Bandwidth (Mbps)\n";
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  return 0;
}
