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
    std::string name = "Rotterdam";

    montblanc::Node node = montblanc::Node(name.c_str());
    node.init();

    using namespace std::chrono;

    auto mekong_next = steady_clock::now();
    auto mekong_prev = steady_clock::now();
    auto mekong_now = steady_clock::now();

    // PUB =========================================================================================
    DataWriter* murray_writer = node.create_datawriter(
      "/murray",
      static_cast<TypeSupport>(new Vector3StampedPubSubType()));
    Vector3Stamped murray_msg;

    // RANDOMIZE ===================================================================================
    printf("%s: Data generation started\n", name.c_str());

    murray_msg = montblanc::random_vector3stamped();

    printf("%s: Data generation done\n\n", name.c_str());

    // SUB =========================================================================================
    DataReader* mekong_reader = node.create_datareader(
      "/mekong",
      static_cast<TypeSupport>(new TwistWithCovarianceStampedPubSubType()),
      [&](DataReader* reader) -> void
      {
        TwistWithCovarianceStamped msg;
        SampleInfo info;

        mekong_prev = mekong_now;
        mekong_now = steady_clock::now();

        if (reader->take_next_sample(&msg, &info) == ReturnCode_t::RETCODE_OK)
        {
          if (info.valid_data)
          {
            printf("%s: Received TwistWithCovarianceStamped<%zu> from /mekong, "
                   "putting Vector3Stamped to /murray | <%ld μs>\n",
                   name.c_str(),
                   msg.twist().covariance().size(),
                   duration_cast<microseconds>(mekong_now - mekong_prev).count());
            murray_writer->write(&murray_msg);
          }
        }
      }
    );

    // LOOP ========================================================================================
    mekong_next = steady_clock::now();
    mekong_prev = steady_clock::now();
    mekong_now = steady_clock::now();

    printf("%s: Starting loop\n", name.c_str());

    while (true)
    {
        std::this_thread::sleep_for(milliseconds(1000));
    }

    return 0;
}
