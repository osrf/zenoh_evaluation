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
  std::string name = "Monaco";

  montblanc::Node node = montblanc::Node(name.c_str());
  node.init();

  using namespace std::chrono;

  auto congo_next = steady_clock::now();
  auto congo_prev = steady_clock::now();
  auto congo_now = steady_clock::now();

  // PUB =========================================================================================
  DataWriter * ohio_writer = node.create_datawriter(
    "/ohio",
    static_cast<TypeSupport>(new Float32PubSubType()));
  Float32 ohio_msg;

  // RANDOMIZE ===================================================================================
  printf("%s: Data generation started\n", name.c_str());

  ohio_msg.data(montblanc::random_number<float>());

  printf("%s: Data generation done\n\n", name.c_str());

  // SUB =========================================================================================
  DataReader * congo_reader = node.create_datareader(
    "/congo",
    static_cast<TypeSupport>(new TwistPubSubType()),
    [&](DataReader * reader) -> void
    {
      Twist msg;
      SampleInfo info;

      congo_prev = congo_now;
      congo_now = steady_clock::now();

      if (reader->take_next_sample(&msg, &info) == ReturnCode_t::RETCODE_OK)
      {
        if (info.valid_data)
        {
          printf("%s: Received Twist from /congo, putting Float32 to /ohio | <%ld μs>\n",
                 name.c_str(),
                 duration_cast<microseconds>(congo_now - congo_prev).count());
          ohio_writer->write(&ohio_msg);
        }
      }
    }
  );

  // LOOP ========================================================================================
  congo_next = steady_clock::now();
  congo_prev = steady_clock::now();
  congo_now = steady_clock::now();

  printf("%s: Starting loop\n", name.c_str());

  while (true)
  {
      std::this_thread::sleep_for(milliseconds(1000));
  }

  return 0;
}
