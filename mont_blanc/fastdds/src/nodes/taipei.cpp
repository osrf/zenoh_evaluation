#include <iostream>
#include <chrono>
#include <thread>

#include "Node.h"
#include "utils.hpp"

#include "types/datatypes.h"
#include "types/datatypesPubSubTypes.h"


using namespace eprosima::fastdds::dds;

int main() {
    std::string name = "Taipei";

    montblanc::Node node = montblanc::Node(name.c_str());
    node.init();

    using namespace std::chrono;

    auto columbia_next = steady_clock::now();
    auto columbia_prev = steady_clock::now();
    auto columbia_now = steady_clock::now();

    // PUB =============================================================================================================
    DataWriter* colorado_writer = node.create_datawriter("/colorado", static_cast<TypeSupport>(new ImagePubSubType()));
    Image colorado_msg;

    // RANDOMIZE =======================================================================================================
    printf("%s: Data generation started\n", name.c_str());

    colorado_msg = montblanc::random_image();

    printf("%s: Data generation done\n\n", name.c_str());

    // SUB =============================================================================================================
    DataReader* columbia_reader = node.create_datareader(
      "/columbia",
      static_cast<TypeSupport>(new ImagePubSubType()),
      [&](DataReader* reader) -> void
      {
        Image msg;
        SampleInfo info;

        columbia_prev = columbia_now;
        columbia_now = steady_clock::now();

        if (reader->take_next_sample(&msg, &info) == ReturnCode_t::RETCODE_OK)
        {
          if (info.valid_data)
          {
            printf("%s: Received Image<%zu> from /columbia, putting Image<%zu> to /colorado | <%ld μs>\n",
                   name.c_str(),
                   msg.data().size(),
                   colorado_msg.data().size(),
                   duration_cast<microseconds>(columbia_now - columbia_prev).count());
           colorado_writer->write(&colorado_msg);
          }
        }
      }
    );

    // LOOP ============================================================================================================
    columbia_next = steady_clock::now();
    columbia_prev = steady_clock::now();
    columbia_now = steady_clock::now();

    printf("%s: Starting loop\n", name.c_str());

    while (true)
    {
        std::this_thread::sleep_for(milliseconds(1000));
    }

    return 0;
}
