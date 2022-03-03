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
  std::string name = "Geneva";

  montblanc::Node node = montblanc::Node(name.c_str());
  node.init();

  using namespace std::chrono;

  auto parana_next = steady_clock::now();
  auto parana_prev = steady_clock::now();
  auto parana_now = steady_clock::now();

  auto danube_next = steady_clock::now();
  auto danube_prev = steady_clock::now();
  auto danube_now = steady_clock::now();

  auto tagus_next = steady_clock::now();
  auto tagus_prev = steady_clock::now();
  auto tagus_now = steady_clock::now();

  auto congo_next = steady_clock::now();
  auto congo_prev = steady_clock::now();
  auto congo_now = steady_clock::now();

  // PUB =========================================================================================
  DataWriter * arkansas_writer = node.create_datawriter(
    "/arkansas",
    static_cast<TypeSupport>(new StringPubSubType()));
  String arkansas_msg;

  // RANDOMIZE ===================================================================================
  printf("%s: Data generation started\n", name.c_str());

  arkansas_msg.data(montblanc::random_string(256));

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

      if (reader->take_next_sample(&msg, &info) == ReturnCode_t::RETCODE_OK) {
        if (info.valid_data) {
          printf(
            "%s: Received String<%zu> from /parana, putting String<%zu> to /arkansas "
            "| <%ld μs>\n",
            name.c_str(),
            msg.data().size(),
            arkansas_msg.data().size(),
            duration_cast<microseconds>(parana_now - parana_prev).count());
          arkansas_writer->write(&arkansas_msg);
        }
      }
    }
  );

  DataReader * danube_reader = node.create_datareader(
    "/danube",
    static_cast<TypeSupport>(new StringPubSubType()),
    [&](DataReader * reader) -> void
    {
      String msg;
      SampleInfo info;

      danube_prev = danube_now;
      danube_now = steady_clock::now();

      if (reader->take_next_sample(&msg, &info) == ReturnCode_t::RETCODE_OK) {
        if (info.valid_data) {
          printf(
            "%s: Received String<%zu> from /danube | <%ld μs>\n",
            name.c_str(),
            msg.data().size(),
            duration_cast<microseconds>(danube_now - danube_prev).count());
        }
      }
    }
  );

  DataReader * tagus_reader = node.create_datareader(
    "/tagus",
    static_cast<TypeSupport>(new PosePubSubType()),
    [&](DataReader * reader) -> void
    {
      Pose msg;
      SampleInfo info;

      tagus_prev = tagus_now;
      tagus_now = steady_clock::now();

      if (reader->take_next_sample(&msg, &info) == ReturnCode_t::RETCODE_OK) {
        if (info.valid_data) {
          printf(
            "%s: Received Pose from /tagus | <%ld μs>\n",
            name.c_str(),
            duration_cast<microseconds>(tagus_now - tagus_prev).count());
        }
      }
    }
  );

  DataReader * congo_reader = node.create_datareader(
    "/congo",
    static_cast<TypeSupport>(new TwistPubSubType()),
    [&](DataReader * reader) -> void
    {
      Twist msg;
      SampleInfo info;

      congo_prev = congo_now;
      congo_now = steady_clock::now();

      if (reader->take_next_sample(&msg, &info) == ReturnCode_t::RETCODE_OK) {
        if (info.valid_data) {
          printf(
            "%s: Received Twist from /congo | <%ld μs>\n",
            name.c_str(),
            duration_cast<microseconds>(congo_now - congo_prev).count());
        }
      }
    }
  );

  // LOOP ========================================================================================
  parana_next = steady_clock::now();
  parana_prev = steady_clock::now();
  parana_now = steady_clock::now();

  danube_next = steady_clock::now();
  danube_prev = steady_clock::now();
  danube_now = steady_clock::now();

  tagus_next = steady_clock::now();
  tagus_prev = steady_clock::now();
  tagus_now = steady_clock::now();

  congo_next = steady_clock::now();
  congo_prev = steady_clock::now();
  congo_now = steady_clock::now();

  printf("%s: Starting loop\n", name.c_str());

  while (true) {
    std::this_thread::sleep_for(milliseconds(1000));
  }

  return 0;
}
