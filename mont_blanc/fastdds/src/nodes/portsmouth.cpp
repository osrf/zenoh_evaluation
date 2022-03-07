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
  std::string name = "Portsmouth";

  montblanc::Node node = montblanc::Node(name.c_str());
  node.init();

  using namespace std::chrono;

  auto next = steady_clock::now();
  auto prev = steady_clock::now();
  auto now = steady_clock::now();

  // PUB =========================================================================================
  DataWriter * danube_writer = node.create_datawriter(
    "/danube",
    static_cast<TypeSupport>(new StringPubSubType()));
  String danube_msg;

  // RANDOMIZE ===================================================================================
  printf("%s: Data generation started\n", name.c_str());

  danube_msg.data(montblanc::random_string(256));

  printf("%s: Data generation done\n\n", name.c_str());

  // LOOP ========================================================================================
  next = steady_clock::now();
  prev = steady_clock::now();
  now = steady_clock::now();

  printf("%s: Starting loop\n", name.c_str());

  while (true) {
    prev = now;
    now = steady_clock::now();
    next = now + milliseconds(200);

    printf(
      "%s: Putting generated String<%zu> to /danube | <%ld μs>\n",
      name.c_str(),
      danube_msg.data().size(),
      duration_cast<microseconds>(now - prev).count());
    danube_writer->write(&danube_msg);

    std::this_thread::sleep_until(next);
  }

  return 0;
}
