#include <iostream>
#include <chrono>
#include <thread>

#include "Node.h"
#include "utils.hpp"

#include "types/datatypes.h"
#include "types/datatypesPubSubTypes.h"


using namespace eprosima::fastdds::dds;

int main() {
    std::string name = "Lyon";

    montblanc::Node node = montblanc::Node(name.c_str());
    node.init();

    using namespace std::chrono;

    auto next = steady_clock::now();
    auto prev = steady_clock::now();
    auto now = steady_clock::now();

    // PUB =============================================================================================================
    DataWriter* tigris_writer = node.create_datawriter("/tigris", static_cast<TypeSupport>(new Float32PubSubType()));
    Float32 tigris_msg;

    // RANDOMIZE =======================================================================================================
    printf("%s: Data generation started\n", name.c_str());

    tigris_msg.data(montblanc::random_number<float>());

    printf("%s: Data generation done\n\n", name.c_str());

    // SUB =============================================================================================================
    DataReader* amazon_reader = node.create_datareader(
      "/amazon",
      static_cast<TypeSupport>(new Float32PubSubType()),
      [&](DataReader* reader) -> void
      {
        Float32 msg;
        SampleInfo info;

        prev = now;
        now = steady_clock::now();

        if (reader->take_next_sample(&msg, &info) == ReturnCode_t::RETCODE_OK)
        {
          if (info.valid_data)
          {
            printf("%s: Received Float32 from /amazon, putting Float32 to /tigris | <%ld μs>\n",
                   name.c_str(),
                   duration_cast<microseconds>(now - prev).count());
           tigris_writer->write(&tigris_msg);
          }
        }
      }
    );

    // LOOP ============================================================================================================
    next = steady_clock::now();
    prev = steady_clock::now();
    now = steady_clock::now();

    printf("%s: Starting loop\n", name.c_str());

    while (true)
    {
        std::this_thread::sleep_for(milliseconds(1000));
    }

    return 0;
}
