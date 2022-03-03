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
    std::string name = "Georgetown";

    montblanc::Node node = montblanc::Node(name.c_str());
    node.init();

    using namespace std::chrono;

    auto murray_next = steady_clock::now();
    auto murray_prev = steady_clock::now();
    auto murray_now = steady_clock::now();

    auto lena_next = steady_clock::now();
    auto lena_prev = steady_clock::now();
    auto lena_now = steady_clock::now();

    auto pub_next = steady_clock::now();
    auto pub_prev = steady_clock::now();
    auto pub_now = steady_clock::now();

    // PUB =========================================================================================
    DataWriter* volga_writer = node.create_datawriter(
      "/volga",
      static_cast<TypeSupport>(new Float64PubSubType()));
    Float64 volga_msg;

    // RANDOMIZE ===================================================================================
    printf("%s: Data generation started\n", name.c_str());

    volga_msg.data(montblanc::random_number<double>());

    printf("%s: Data generation done\n\n", name.c_str());

    // SUB =========================================================================================
    DataReader* murray_reader = node.create_datareader(
      "/murray",
      static_cast<TypeSupport>(new Vector3StampedPubSubType()),
      [&](DataReader* reader) -> void
      {
        Vector3Stamped msg;
        SampleInfo info;

        murray_prev = murray_now;
        murray_now = steady_clock::now();

        if (reader->take_next_sample(&msg, &info) == ReturnCode_t::RETCODE_OK)
        {
          if (info.valid_data)
          {
            printf("%s: Received Vector3Stamped from /murray | <%ld μs>\n",
                   name.c_str(),
                   duration_cast<microseconds>(murray_now - murray_prev).count());
          }
        }
      }
    );

    DataReader* lena_reader = node.create_datareader(
      "/lena",
      static_cast<TypeSupport>(new WrenchStampedPubSubType()),
      [&](DataReader* reader) -> void
      {
        WrenchStamped msg;
        SampleInfo info;

        lena_prev = lena_now;
        lena_now = steady_clock::now();

        if (reader->take_next_sample(&msg, &info) == ReturnCode_t::RETCODE_OK)
        {
          if (info.valid_data)
          {
            printf("%s: Received WrenchStamped from /lena | <%ld μs>\n",
                   name.c_str(),
                   duration_cast<microseconds>(lena_now - lena_prev).count());
          }
        }
      }
    );

    // LOOP ========================================================================================
    murray_next = steady_clock::now();
    murray_prev = steady_clock::now();
    murray_now = steady_clock::now();

    lena_next = steady_clock::now();
    lena_prev = steady_clock::now();
    lena_now = steady_clock::now();

    pub_next = steady_clock::now();
    pub_prev = steady_clock::now();
    pub_now = steady_clock::now();

    printf("%s: Starting loop\n", name.c_str());

    while (true)
    {
      pub_prev = pub_now;
      pub_now = steady_clock::now();
      pub_next = pub_now + milliseconds(50);

      printf("%s: Putting generated Float64 to /volga | <%ld μs>\n",
             name.c_str(),
             duration_cast<microseconds>(pub_now - pub_prev).count());

      volga_writer->write(&volga_msg);

      std::this_thread::sleep_until(pub_next);
    }

    return 0;
}
