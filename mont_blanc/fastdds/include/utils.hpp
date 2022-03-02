#ifndef _UTILS_H_
#define _UTILS_H_

#include <random>
#include <vector>
#include <algorithm>
#include <limits>

#include "types/datatypes.h"


namespace montblanc
{

// Modified from: https://stackoverflow.com/questions/440133/how-do-i-create-a-random-alpha-numeric-string-in-c
std::string random_string( size_t length )
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
        return charset[ rand() % max_index ];
    };
    std::string str(length, 0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}

template <typename T,
          typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type >
T random_number(T min = std::numeric_limits<T>::min(), T max = std::numeric_limits<T>::max())
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dist(min, max);

  return dist(gen);
}

template <typename T,
          typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type >
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

// MSG GEN ==============================================================================================================
Header random_header(size_t len = 16)
{
    Header header_msg;
    header_msg.sec(random_number<long>());
    header_msg.nanosec(random_number<unsigned long>());
    header_msg.frame_id(random_string(len));

    return header_msg;
}

Image random_image(size_t len = 0) // 1920 * 1080 * 3
{
    Image image_msg;

    image_msg.header(montblanc::random_header());

    image_msg.height(montblanc::random_number<unsigned long>());
    image_msg.width(montblanc::random_number<unsigned long>());

    image_msg.encoding(montblanc::random_string(32));
    image_msg.is_bigendian(montblanc::random_number<int>(0, 1));
    image_msg.step(montblanc::random_number<unsigned long>());

    image_msg.data(montblanc::random_number_vector<uint8_t>(len));

    return image_msg;
}

PointField random_pointfield(size_t len = 32)
{
    PointField pt_msg;

    pt_msg.name(montblanc::random_string(len));
    pt_msg.offset(montblanc::random_number<unsigned long>());
    pt_msg.datatype(montblanc::random_number<uint8_t>());
    pt_msg.count(montblanc::random_number<unsigned long>(0, 1));

    return pt_msg;
}

PointCloud2 random_pointcloud(size_t len = 0) // 4 * 4 * 4 * 1280 * 960
{
    PointCloud2 pc_msg;

    pc_msg.header(montblanc::random_header());

    pc_msg.height(montblanc::random_number<unsigned long>());
    pc_msg.width(montblanc::random_number<unsigned long>());

    std::vector<PointField> pts(3);
    for (int i=0; i < 3; ++i)
    {
        pts.push_back(random_pointfield());
    }

    pc_msg.fields(pts);

    pc_msg.is_bigendian(montblanc::random_number<int>(0, 1));
    pc_msg.point_step(montblanc::random_number<unsigned long>());
    pc_msg.row_step(montblanc::random_number<unsigned long>());

    pc_msg.data(montblanc::random_number_vector<uint8_t>(len));

    pc_msg.is_dense(montblanc::random_number<int>(0, 1));

    return pc_msg;
}


} // namespace montblanc

#endif // _UTILS_H
