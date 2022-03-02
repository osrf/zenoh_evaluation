#include <iostream>
#include <chrono>
#include <thread>

#include "Node.h"
#include "utils.hpp"

#include "types/datatypes.h"
#include "types/datatypesPubSubTypes.h"


using namespace eprosima::fastdds::dds;

int main() {
    std::string name = "Cordoba";

    montblanc::Node node = montblanc::Node(name.c_str());
    node.init();

    using namespace std::chrono;

    auto next = steady_clock::now();
    auto prev = steady_clock::now();
    auto now = steady_clock::now();

    // PUB =============================================================================================================
    DataWriter* amazon_writer = node.create_datawriter("/amazon", static_cast<TypeSupport>(new Float32PubSubType()));
    Float32 amazon_msg;

    // RANDOMIZE =======================================================================================================
    printf("%s: Data generation started\n", name.c_str());

    amazon_msg.data(montblanc::random_number<float>());

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

      printf("%s: Putting generated Float32 to /amazon | <%ld μs>\n",
             name.c_str(),
             duration_cast<microseconds>(now - prev).count());
      amazon_writer->write(&amazon_msg);

      std::this_thread::sleep_until(next);
    }

    return 0;
}
