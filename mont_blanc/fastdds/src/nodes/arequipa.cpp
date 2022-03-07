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
  std::string name = "Arequipa";

  montblanc::Node node = montblanc::Node(name.c_str());
  node.init();

  using namespace std::chrono;

  auto arkansas_next = steady_clock::now();
  auto arkansas_prev = steady_clock::now();
  auto arkansas_now = steady_clock::now();

  // PUB =========================================================================================
  // NONE

  // RANDOMIZE ===================================================================================
  // NONE

  // SUB =========================================================================================
  DataReader * arkansas_reader = node.create_datareader(
    "/arkansas",
    static_cast<TypeSupport>(new StringPubSubType()),
    [&](DataReader * reader) -> void
    {
      String msg;
      SampleInfo info;

      arkansas_prev = arkansas_now;
      arkansas_now = steady_clock::now();

      if (reader->take_next_sample(&msg, &info) == ReturnCode_t::RETCODE_OK) {
        if (info.valid_data) {
          printf(
            "%s: Received String<%zu> from /arkansas | <%ld μs>\n",
            name.c_str(),
            msg.data().size(),
            duration_cast<microseconds>(arkansas_now - arkansas_prev).count());
        }
      }
    }
  );

  // LOOP ========================================================================================
  arkansas_next = steady_clock::now();
  arkansas_prev = steady_clock::now();
  arkansas_now = steady_clock::now();

  printf("%s: Starting loop\n", name.c_str());

  while (true) {
    std::this_thread::sleep_for(milliseconds(1000));
  }

  return 0;
}
