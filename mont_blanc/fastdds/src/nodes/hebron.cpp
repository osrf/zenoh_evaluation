#include <iostream>
#include <chrono>
#include <thread>

#include "Node.h"
#include "utils.hpp"

#include "types/datatypes.h"
#include "types/datatypesPubSubTypes.h"


using namespace eprosima::fastdds::dds;

int main() {
    std::string name = "Hebron";

    montblanc::Node node = montblanc::Node(name.c_str());
    node.init();

    using namespace std::chrono;

    auto chenab_next = steady_clock::now();
    auto chenab_prev = steady_clock::now();
    auto chenab_now = steady_clock::now();

    // PUB =============================================================================================================
    DataWriter* chenab_writer = node.create_datawriter("/chenab", static_cast<TypeSupport>(new QuaternionPubSubType()));
    Quaternion chenab_msg;

    // RANDOMIZE =======================================================================================================
    printf("%s: Data generation started\n", name.c_str());

    chenab_msg = montblanc::random_quaternion();

    printf("%s: Data generation done\n\n", name.c_str());

    // LOOP ============================================================================================================
    chenab_next = steady_clock::now();
    chenab_prev = steady_clock::now();
    chenab_now = steady_clock::now();

    printf("%s: Starting loop\n", name.c_str());

    while (true)
    {
      chenab_prev = chenab_now;
      chenab_now = steady_clock::now();
      chenab_next = chenab_now + milliseconds(100);

      printf("%s: Putting generated Quaternion to /chenab | <%ld μs>\n",
             name.c_str(),
             duration_cast<microseconds>(chenab_now - chenab_prev).count());
      chenab_writer->write(&chenab_msg);

      std::this_thread::sleep_until(chenab_next);
    }

    return 0;
}
