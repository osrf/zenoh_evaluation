#include <iostream>
#include <chrono>
#include <thread>

#include "Node.h"
#include "utils.hpp"

#include "types/datatypes.h"
#include "types/datatypesPubSubTypes.h"


using namespace eprosima::fastdds::dds;

int main() {
    std::string name = "Delhi";

    montblanc::Node node = montblanc::Node(name.c_str());
    node.init();

    using namespace std::chrono;

    auto next = steady_clock::now();
    auto prev = steady_clock::now();
    auto now = steady_clock::now();

    // PUB =============================================================================================================
    DataWriter* columbia_writer = node.create_datawriter("/columbia", static_cast<TypeSupport>(new ImagePubSubType()));
    Image columbia_msg;

    // RANDOMIZE =======================================================================================================
    printf("%s: Data generation started\n", name.c_str());

    columbia_msg = montblanc::random_image();

    printf("%s: Data generation done\n\n", name.c_str());

    // LOOP ============================================================================================================
    next = steady_clock::now();
    prev = steady_clock::now();
    now = steady_clock::now();

    printf("%s: Starting loop\n", name.c_str());

    while (true)
    {
      prev = now;
      now = steady_clock::now();
      next = now + milliseconds(100);

      printf("%s: Putting generated Image<%ld> to /columbia | <%ld μs>\n",
             name.c_str(),
             columbia_msg.data().size(),
             duration_cast<microseconds>(now - prev).count());
      columbia_writer->write(&columbia_msg);

      std::this_thread::sleep_until(next);
    }

    return 0;
}
