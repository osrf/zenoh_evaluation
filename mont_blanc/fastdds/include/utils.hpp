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

#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <random>
#include <vector>
#include <algorithm>
#include <limits>
#include <string>

#include "types/datatypes.h"


namespace montblanc
{

// Modified from:
// https://stackoverflow.com/questions/440133/how-do-i-create-a-random-alpha-numeric-string-in-c
std::string random_string(size_t length)
{
  auto randchar = []() -> char
    {
      const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "`~!@#$%^&*()_+"
        "[]\\;',./{}|:\"<>?";
      const size_t max_index = (sizeof(charset) - 1);
      return charset[rand() % max_index];
    };
  std::string str(length, 0);
  std::generate_n(str.begin(), length, randchar);
  return str;
}

template<typename T,
  typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
T random_number(T min = std::numeric_limits<T>::min(), T max = std::numeric_limits<T>::max())
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dist(min, max);

  return dist(gen);
}

template<typename T,
  typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
std::vector<T> random_number_vector(
  size_t length,
  T min = std::numeric_limits<T>::min(),
  T max = std::numeric_limits<T>::max())
{
  auto randnum = [&]() -> T
    {
      return random_number<T>(min, max);
    };

  std::vector<T> out(length);
  std::generate(out.begin(), out.end(), randnum);

  return out;
}

// MSG GEN =========================================================================================
Header random_header(size_t len = 16)
{
  Header header_msg;
  header_msg.sec(random_number<int32_t>());
  header_msg.nanosec(random_number<uint32_t>());
  header_msg.frame_id(random_string(len));

  return header_msg;
}

Quaternion random_quaternion()
{
  Quaternion q_msg;

  q_msg.x(montblanc::random_number<double>());
  q_msg.y(montblanc::random_number<double>());
  q_msg.z(montblanc::random_number<double>());
  q_msg.w(montblanc::random_number<double>());

  return q_msg;
}

Point random_point()
{
  Point p_msg;

  p_msg.x(montblanc::random_number<double>());
  p_msg.y(montblanc::random_number<double>());
  p_msg.z(montblanc::random_number<double>());

  return p_msg;
}

Pose random_pose()
{
  Pose pose_msg;

  pose_msg.position(montblanc::random_point());
  pose_msg.orientation(montblanc::random_quaternion());

  return pose_msg;
}

Vector3 random_vector3()
{
  Vector3 vec_msg;

  vec_msg.x(montblanc::random_number<double>());
  vec_msg.y(montblanc::random_number<double>());
  vec_msg.z(montblanc::random_number<double>());

  return vec_msg;
}

Vector3Stamped random_vector3stamped()
{
  Vector3Stamped vec_msg;

  vec_msg.header(montblanc::random_header());
  vec_msg.vector(montblanc::random_vector3());

  return vec_msg;
}

Twist random_twist()
{
  Twist twist_msg;

  twist_msg.linear(montblanc::random_vector3());
  twist_msg.angular(montblanc::random_vector3());

  return twist_msg;
}

TwistWithCovariance random_twistwithcovariance(size_t len = 36)
{
  TwistWithCovariance twist_msg;

  twist_msg.twist(montblanc::random_twist());
  twist_msg.covariance(montblanc::random_number_vector<double>(len));

  return twist_msg;
}

TwistWithCovarianceStamped random_twistwithcovariancestamped(size_t len = 36)
{
  TwistWithCovarianceStamped twist_msg;

  twist_msg.header(montblanc::random_header());
  twist_msg.twist(montblanc::random_twistwithcovariance(len));

  return twist_msg;
}

Wrench random_wrench()
{
  Wrench wrench_msg;

  wrench_msg.force(montblanc::random_vector3());
  wrench_msg.torque(montblanc::random_vector3());

  return wrench_msg;
}

WrenchStamped random_wrenchstamped()
{
  WrenchStamped wrench_msg;

  wrench_msg.header(montblanc::random_header());
  wrench_msg.wrench(montblanc::random_wrench());

  return wrench_msg;
}

Image random_image(size_t len = 0)  // 1920 * 1080 * 3
{
  Image image_msg;

  image_msg.header(montblanc::random_header());

  image_msg.height(montblanc::random_number<uint32_t>());
  image_msg.width(montblanc::random_number<uint32_t>());

  image_msg.encoding(montblanc::random_string(32));
  image_msg.is_bigendian(montblanc::random_number<int>(0, 1));
  image_msg.step(montblanc::random_number<uint32_t>());

  image_msg.data(montblanc::random_number_vector<uint8_t>(len));

  return image_msg;
}

PointField random_pointfield(size_t len = 32)
{
  PointField pt_msg;

  pt_msg.name(montblanc::random_string(len));
  pt_msg.offset(montblanc::random_number<uint32_t>());
  pt_msg.datatype(montblanc::random_number<uint8_t>());
  pt_msg.count(montblanc::random_number<uint32_t>(0, 1));

  return pt_msg;
}

PointCloud2 random_pointcloud(size_t len = 0)  // 4 * 4 * 4 * 1280 * 960
{
  PointCloud2 pc_msg;

  pc_msg.header(montblanc::random_header());

  pc_msg.height(montblanc::random_number<uint32_t>());
  pc_msg.width(montblanc::random_number<uint32_t>());

  std::vector<PointField> pts(3);
  for (int i = 0; i < 3; ++i) {
    pts.push_back(random_pointfield());
  }

  pc_msg.fields(pts);

  pc_msg.is_bigendian(montblanc::random_number<int>(0, 1));
  pc_msg.point_step(montblanc::random_number<uint32_t>());
  pc_msg.row_step(montblanc::random_number<uint32_t>());

  pc_msg.data(montblanc::random_number_vector<uint8_t>(len));

  pc_msg.is_dense(montblanc::random_number<int>(0, 1));

  return pc_msg;
}

LaserScan random_laserscan(size_t len = 1024)
{
  LaserScan scan_msg;

  scan_msg.header(montblanc::random_header());

  scan_msg.angle_min(montblanc::random_number<float>());
  scan_msg.angle_max(montblanc::random_number<float>());
  scan_msg.angle_increment(montblanc::random_number<float>());
  scan_msg.time_increment(montblanc::random_number<float>());
  scan_msg.scan_time(montblanc::random_number<float>());
  scan_msg.range_min(montblanc::random_number<float>());
  scan_msg.range_max(montblanc::random_number<float>());

  scan_msg.ranges(montblanc::random_number_vector<float>(len));
  scan_msg.intensities(montblanc::random_number_vector<float>(len));

  return scan_msg;
}


}  // namespace montblanc

#endif  // UTILS_HPP_
