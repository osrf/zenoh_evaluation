#ifndef _UTILS_H_
#define _UTILS_H_

#include "types/datatypes.h"

#include <random>
#include <algorithm>
#include <limits>

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
    std::string str(length,0);
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

Header random_header(int len = 16)
{
    Header header_msg;
    header_msg.sec(random_number<long>());
    header_msg.nanosec(random_number<unsigned long>());
    header_msg.frame_id(random_string(len));

    return header_msg;
}

} // namespace montblanc

#endif // _UTILS_H
