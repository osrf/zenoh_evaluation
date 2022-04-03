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
  std::string name = "Publisher";

  std::unique_ptr<StdoutConsumer> stdout_consumer(new StdoutConsumer());
  Log::RegisterConsumer(std::move(stdout_consumer));
  Log::SetVerbosity(Log::Kind::Info);

  dds_node::Node node = dds_node::Node(name.c_str());
  node.init();

  // PUB =========================================================================================
  DataWriter * writer = node.create_datawriter(
    "/amazon",
    static_cast<TypeSupport>(new BigDataPubSubType()));
  BigData msg;

  // RANDOMIZE ===================================================================================
  std::cout << name << ": Data generation started\n";

  msg = dds_types::random_bigdata(false);

  std::cout << name << ": Data generation done\n";

  // LOOP ========================================================================================
  std::cout << name << ": Starting loop\n";
  while (true) {
    std::cout << name << ": Putting generated data to /amazon\n";

    dds_types::set_bigdata_timestamp_to_now(msg);
    auto start_time = std::chrono::high_resolution_clock::now();
    writer->write(&msg);
    auto end_time = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> time_taken = end_time - start_time;
    std::cout << name << ": Data write took " << time_taken.count() << " ms\n";

    auto sleep_until_time = start_time + std::chrono::milliseconds(100);
    std::this_thread::sleep_until(sleep_until_time);
  }

  return 0;
}
