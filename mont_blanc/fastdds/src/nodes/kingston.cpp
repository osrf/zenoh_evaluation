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
    std::string name = "Kingston";

    montblanc::Node node = montblanc::Node(name.c_str());
    node.init();

    using namespace std::chrono;

    auto yamuna_next = steady_clock::now();
    auto yamuna_prev = steady_clock::now();
    auto yamuna_now = steady_clock::now();

    // PUB =========================================================================================
    DataWriter* yamuna_writer = node.create_datawriter(
      "/yamuna",
      static_cast<TypeSupport>(new Vector3PubSubType()));
    Vector3 yamuna_msg;

    // RANDOMIZE ===================================================================================
    printf("%s: Data generation started\n", name.c_str());

    yamuna_msg = montblanc::random_vector3();

    printf("%s: Data generation done\n\n", name.c_str());

    // LOOP ========================================================================================
    yamuna_next = steady_clock::now();
    yamuna_prev = steady_clock::now();
    yamuna_now = steady_clock::now();

    printf("%s: Starting loop\n", name.c_str());

    while (true)
    {
      yamuna_prev = yamuna_now;
      yamuna_now = steady_clock::now();
      yamuna_next = yamuna_now + milliseconds(100);

      printf("%s: Putting generated Vector3 to /yamuna | <%ld μs>\n",
             name.c_str(),
             duration_cast<microseconds>(yamuna_now - yamuna_prev).count());
      yamuna_writer->write(&yamuna_msg);

      std::this_thread::sleep_until(yamuna_next);
    }

    return 0;
}
