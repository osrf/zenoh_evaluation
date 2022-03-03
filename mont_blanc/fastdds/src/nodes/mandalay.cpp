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
  std::string name = "Mandalay";

  montblanc::Node node = montblanc::Node(name.c_str());
  node.init();

  using namespace std::chrono;

  auto danube_next = steady_clock::now();
  auto danube_prev = steady_clock::now();
  auto danube_now = steady_clock::now();

  auto chenab_next = steady_clock::now();
  auto chenab_prev = steady_clock::now();
  auto chenab_now = steady_clock::now();

  auto salween_next = steady_clock::now();
  auto salween_prev = steady_clock::now();
  auto salween_now = steady_clock::now();

  auto godavari_next = steady_clock::now();
  auto godavari_prev = steady_clock::now();
  auto godavari_now = steady_clock::now();

  auto yamuna_next = steady_clock::now();
  auto yamuna_prev = steady_clock::now();
  auto yamuna_now = steady_clock::now();

  auto loire_next = steady_clock::now();
  auto loire_prev = steady_clock::now();
  auto loire_now = steady_clock::now();

  auto pub_next = steady_clock::now();
  auto pub_prev = steady_clock::now();
  auto pub_now = steady_clock::now();

  // PUB =========================================================================================
  DataWriter * tagus_writer = node.create_datawriter(
    "/tagus",
    static_cast<TypeSupport>(new PosePubSubType()));
  Pose tagus_msg;

  DataWriter * missouri_writer = node.create_datawriter(
    "/missouri",
    static_cast<TypeSupport>(new ImagePubSubType()));
  Image missouri_msg;

  DataWriter * brazos_writer = node.create_datawriter(
    "/brazos",
    static_cast<TypeSupport>(new PointCloud2PubSubType()));
  PointCloud2 brazos_msg;

  // RANDOMIZE ===================================================================================
  printf("%s: Data generation started\n", name.c_str());

  tagus_msg = montblanc::random_pose();
  missouri_msg = montblanc::random_image();
  brazos_msg = montblanc::random_pointcloud();

  printf("%s: Data generation done\n\n", name.c_str());

  // SUB =========================================================================================
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

  DataReader * chenab_reader = node.create_datareader(
    "/chenab",
    static_cast<TypeSupport>(new QuaternionPubSubType()),
    [&](DataReader * reader) -> void
    {
      Quaternion msg;
      SampleInfo info;

      chenab_prev = chenab_now;
      chenab_now = steady_clock::now();

      if (reader->take_next_sample(&msg, &info) == ReturnCode_t::RETCODE_OK) {
        if (info.valid_data) {
          printf(
            "%s: Received Quaternion from /chenab | <%ld μs>\n",
            name.c_str(),
            duration_cast<microseconds>(chenab_now - chenab_prev).count());
        }
      }
    }
  );

  DataReader * salween_reader = node.create_datareader(
    "/salween",
    static_cast<TypeSupport>(new PointCloud2PubSubType()),
    [&](DataReader * reader) -> void
    {
      PointCloud2 msg;
      SampleInfo info;

      salween_prev = salween_now;
      salween_now = steady_clock::now();

      if (reader->take_next_sample(&msg, &info) == ReturnCode_t::RETCODE_OK) {
        if (info.valid_data) {
          printf(
            "%s: Received PointCloud2<%zu> from /salween | <%ld μs>\n",
            name.c_str(),
            msg.data().size(),
            duration_cast<microseconds>(salween_now - salween_prev).count());
        }
      }
    }
  );

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
            "%s: Received LaserScan<%zu, %zu> from /godavari | <%ld μs>\n",
            name.c_str(),
            msg.ranges().size(),
            msg.intensities().size(),
            duration_cast<microseconds>(godavari_now - godavari_prev).count());
        }
      }
    }
  );

  DataReader * yamuna_reader = node.create_datareader(
    "/yamuna",
    static_cast<TypeSupport>(new Vector3PubSubType()),
    [&](DataReader * reader) -> void
    {
      Vector3 msg;
      SampleInfo info;

      yamuna_prev = yamuna_now;
      yamuna_now = steady_clock::now();

      if (reader->take_next_sample(&msg, &info) == ReturnCode_t::RETCODE_OK) {
        if (info.valid_data) {
          printf(
            "%s: Received Vector3 from /yamuna | <%ld μs>\n",
            name.c_str(),
            duration_cast<microseconds>(yamuna_now - yamuna_prev).count());
        }
      }
    }
  );

  DataReader * loire_reader = node.create_datareader(
    "/loire",
    static_cast<TypeSupport>(new PointCloud2PubSubType()),
    [&](DataReader * reader) -> void
    {
      PointCloud2 msg;
      SampleInfo info;

      loire_prev = loire_now;
      loire_now = steady_clock::now();

      if (reader->take_next_sample(&msg, &info) == ReturnCode_t::RETCODE_OK) {
        if (info.valid_data) {
          printf(
            "%s: Received PointCloud2<%zu> from /loire | <%ld μs>\n",
            name.c_str(),
            msg.data().size(),
            duration_cast<microseconds>(loire_now - loire_prev).count());
        }
      }
    }
  );

  // LOOP ========================================================================================
  danube_next = steady_clock::now();
  danube_prev = steady_clock::now();
  danube_now = steady_clock::now();

  chenab_next = steady_clock::now();
  chenab_prev = steady_clock::now();
  chenab_now = steady_clock::now();

  salween_next = steady_clock::now();
  salween_prev = steady_clock::now();
  salween_now = steady_clock::now();

  godavari_next = steady_clock::now();
  godavari_prev = steady_clock::now();
  godavari_now = steady_clock::now();

  yamuna_next = steady_clock::now();
  yamuna_prev = steady_clock::now();
  yamuna_now = steady_clock::now();

  loire_next = steady_clock::now();
  loire_prev = steady_clock::now();
  loire_now = steady_clock::now();

  pub_next = steady_clock::now();
  pub_prev = steady_clock::now();
  pub_now = steady_clock::now();

  printf("%s: Starting loop\n", name.c_str());

  while (true) {
    pub_prev = pub_now;
    pub_now = steady_clock::now();
    pub_next = pub_now + milliseconds(100);

    printf(
      "%s: Putting generated Pose to /tagus, Image<%zu> to /missouri, "
      "PointCloud2<%zu> to /brazos | <%ld μs>\n",
      name.c_str(),
      missouri_msg.data().size(),
      brazos_msg.data().size(),
      duration_cast<microseconds>(pub_now - pub_prev).count());

    tagus_writer->write(&tagus_msg);
    missouri_writer->write(&missouri_msg);
    brazos_writer->write(&brazos_msg);

    std::this_thread::sleep_until(pub_next);
  }

  return 0;
}
