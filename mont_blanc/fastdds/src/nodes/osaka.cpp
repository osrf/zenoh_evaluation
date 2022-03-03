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

int main() {
  std::string name = "Osaka";

  montblanc::Node node = montblanc::Node(name.c_str());
  node.init();

  using namespace std::chrono;

  auto parana_next = steady_clock::now();
  auto parana_prev = steady_clock::now();
  auto parana_now = steady_clock::now();

  auto columbia_next = steady_clock::now();
  auto columbia_prev = steady_clock::now();
  auto columbia_now = steady_clock::now();

  auto colorado_next = steady_clock::now();
  auto colorado_prev = steady_clock::now();
  auto colorado_now = steady_clock::now();

  // PUB =========================================================================================
  DataWriter * salween_writer = node.create_datawriter(
    "/salween",
    static_cast<TypeSupport>(new PointCloud2PubSubType()));
  PointCloud2 salween_msg;

  DataWriter * godavari_writer = node.create_datawriter(
    "/godavari",
    static_cast<TypeSupport>(new LaserScanPubSubType()));
  LaserScan godavari_msg;

  // RANDOMIZE ===================================================================================
  printf("%s: Data generation started\n", name.c_str());

  salween_msg = montblanc::random_pointcloud();
  godavari_msg = montblanc::random_laserscan();

  printf("%s: Data generation done\n\n", name.c_str());

  // SUB =========================================================================================
  DataReader * parana_reader = node.create_datareader(
    "/parana",
    static_cast<TypeSupport>(new StringPubSubType()),
    [&](DataReader * reader) -> void
    {
      String msg;
      SampleInfo info;

      parana_prev = parana_now;
      parana_now = steady_clock::now();

      if (reader->take_next_sample(&msg, &info) == ReturnCode_t::RETCODE_OK)
      {
        if (info.valid_data)
        {
          printf("%s: Received String<%zu> from /parana | <%ld μs>\n",
                 name.c_str(),
                 msg.data().size(),
                 duration_cast<microseconds>(parana_now - parana_prev).count());
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

      if (reader->take_next_sample(&msg, &info) == ReturnCode_t::RETCODE_OK)
      {
        if (info.valid_data)
        {
          printf("%s: Received Image<%zu> from /columbia | <%ld μs>\n",
                 name.c_str(),
                 msg.data().size(),
                 duration_cast<microseconds>(columbia_now - columbia_prev).count());
        }
      }
    }
  );

  DataReader * colorado_reader = node.create_datareader(
    "/colorado",
    static_cast<TypeSupport>(new ImagePubSubType()),
    [&](DataReader * reader) -> void
    {
      Image msg;
      SampleInfo info;

      colorado_prev = colorado_now;
      colorado_now = steady_clock::now();

      if (reader->take_next_sample(&msg, &info) == ReturnCode_t::RETCODE_OK)
      {
        if (info.valid_data)
        {
          printf("%s: Received Image<%zu> from /colorado, putting PointCloud2<%zu> to /salween, "
                 "putting LaserScan<%zu, %zu> to /godavari | <%ld μs>\n",
                 name.c_str(),
                 msg.data().size(),
                 salween_msg.data().size(),
                 godavari_msg.ranges().size(),
                 godavari_msg.intensities().size(),
                 duration_cast<microseconds>(colorado_now - colorado_prev).count());
          salween_writer->write(&salween_msg);
          godavari_writer->write(&godavari_msg);
        }
      }
    }
  );

  // LOOP ========================================================================================
  parana_next = steady_clock::now();
  parana_prev = steady_clock::now();
  parana_now = steady_clock::now();

  columbia_next = steady_clock::now();
  columbia_prev = steady_clock::now();
  columbia_now = steady_clock::now();

  colorado_next = steady_clock::now();
  colorado_prev = steady_clock::now();
  colorado_now = steady_clock::now();

  printf("%s: Starting loop\n", name.c_str());

  while (true)
  {
      std::this_thread::sleep_for(milliseconds(1000));
  }

  return 0;
}
