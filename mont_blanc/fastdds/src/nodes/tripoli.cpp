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


using namespace eprosima::fastdds::dds;

int main()
{
  std::string name = "Tripoli";

  montblanc::Node node = montblanc::Node(name.c_str());
  node.init();

  using namespace std::chrono;

  auto godavari_next = steady_clock::now();
  auto godavari_prev = steady_clock::now();
  auto godavari_now = steady_clock::now();

  auto columbia_next = steady_clock::now();
  auto columbia_prev = steady_clock::now();
  auto columbia_now = steady_clock::now();

  // PUB =========================================================================================
  DataWriter * loire_writer = node.create_datawriter(
    "/loire",
    static_cast<TypeSupport>(new PointCloud2PubSubType()));
  PointCloud2 loire_msg;

  // RANDOMIZE ===================================================================================
  printf("%s: Data generation started\n", name.c_str());

  loire_msg = montblanc::random_pointcloud();

  printf("%s: Data generation done\n\n", name.c_str());

  // SUB =========================================================================================
  DataReader * godavari_reader = node.create_datareader(
    "/godavari",
    static_cast<TypeSupport>(new LaserScanPubSubType()),
    [&](DataReader * reader) -> void
    {
      LaserScan msg;
      SampleInfo info;

      godavari_prev = godavari_now;
      godavari_now = steady_clock::now();

      if (reader->take_next_sample(&msg, &info) == ReturnCode_t::RETCODE_OK) {
        if (info.valid_data) {
          printf(
            "%s: Received LaserScan<%zu, %zu> from /godavari, "
            "putting PointCloud2<%zu> to /loire, | <%ld μs>\n",
            name.c_str(),
            msg.ranges().size(),
            msg.intensities().size(),
            loire_msg.data().size(),
            duration_cast<microseconds>(godavari_now - godavari_prev).count());
          loire_writer->write(&loire_msg);
        }
      }
    }
  );

  DataReader * columbia_reader = node.create_datareader(
    "/columbia",
    static_cast<TypeSupport>(new ImagePubSubType()),
    [&](DataReader * reader) -> void
    {
      Image msg;
      SampleInfo info;

      columbia_prev = columbia_now;
      columbia_now = steady_clock::now();

      if (reader->take_next_sample(&msg, &info) == ReturnCode_t::RETCODE_OK) {
        if (info.valid_data) {
          printf(
            "%s: Received Image<%zu> from /columbia | <%ld μs>\n",
            name.c_str(),
            msg.data().size(),
            duration_cast<microseconds>(columbia_now - columbia_prev).count());
        }
      }
    }
  );

  // LOOP ========================================================================================
  godavari_next = steady_clock::now();
  godavari_prev = steady_clock::now();
  godavari_now = steady_clock::now();

  columbia_next = steady_clock::now();
  columbia_prev = steady_clock::now();
  columbia_now = steady_clock::now();

  printf("%s: Starting loop\n", name.c_str());

  while (true) {
    std::this_thread::sleep_for(milliseconds(1000));
  }

  return 0;
}
