#include <iostream>
#include <chrono>
#include <thread>

#include "Node.h"
#include "utils.hpp"

#include "types/datatypes.h"
#include "types/datatypesPubSubTypes.h"


using namespace eprosima::fastdds::dds;

int main() {
    std::string name = "Hamburg";

    montblanc::Node node = montblanc::Node(name.c_str());
    node.init();

    using namespace std::chrono;

    auto tigris_next = steady_clock::now();
    auto tigris_prev = steady_clock::now();
    auto tigris_now = steady_clock::now();

    auto ganges_next = steady_clock::now();
    auto ganges_prev = steady_clock::now();
    auto ganges_now = steady_clock::now();

    auto nile_next = steady_clock::now();
    auto nile_prev = steady_clock::now();
    auto nile_now = steady_clock::now();

    auto danube_next = steady_clock::now();
    auto danube_prev = steady_clock::now();
    auto danube_now = steady_clock::now();

    // PUB =============================================================================================================
    DataWriter* parana_writer = node.create_datawriter("/parana", static_cast<TypeSupport>(new StringPubSubType()));
    String parana_msg;

    // RANDOMIZE =======================================================================================================
    printf("%s: Data generation started\n", name.c_str());

    parana_msg.data(montblanc::random_string(256));

    printf("%s: Data generation done\n\n", name.c_str());

    // SUB =============================================================================================================
    DataReader* tigris_reader = node.create_datareader(
      "/tigris",
      static_cast<TypeSupport>(new Float32PubSubType()),
      [&](DataReader* reader) -> void
      {
        Float32 msg;
        SampleInfo info;

        tigris_prev = tigris_now;
        tigris_now = steady_clock::now();

        if (reader->take_next_sample(&msg, &info) == ReturnCode_t::RETCODE_OK)
        {
          if (info.valid_data)
          {
            printf("%s: Received Float32 from /tigris | <%ld μs>\n",
                   name.c_str(),
                   duration_cast<microseconds>(tigris_now - tigris_prev).count());
          }
        }
      }
    );

    DataReader* ganges_reader = node.create_datareader(
      "/ganges",
      static_cast<TypeSupport>(new Int64PubSubType()),
      [&](DataReader* reader) -> void
      {
        Int64 msg;
        SampleInfo info;

        ganges_prev = ganges_now;
        ganges_now = steady_clock::now();

        if (reader->take_next_sample(&msg, &info) == ReturnCode_t::RETCODE_OK)
        {
          if (info.valid_data)
          {
            printf("%s: Received Int64 from /ganges | <%ld μs>\n",
                   name.c_str(),
                   duration_cast<microseconds>(ganges_now - ganges_prev).count());
          }
        }
      }
    );

    DataReader* nile_reader = node.create_datareader(
      "/nile",
      static_cast<TypeSupport>(new Int32PubSubType()),
      [&](DataReader* reader) -> void
      {
        Int32 msg;
        SampleInfo info;

        nile_prev = nile_now;
        nile_now = steady_clock::now();

        if (reader->take_next_sample(&msg, &info) == ReturnCode_t::RETCODE_OK)
        {
          if (info.valid_data)
          {
            printf("%s: Received Int32 from /nile | <%ld μs>\n",
                   name.c_str(),
                   duration_cast<microseconds>(nile_now - nile_prev).count());
          }
        }
      }
    );

    DataReader* danube_reader = node.create_datareader(
      "/danube",
      static_cast<TypeSupport>(new StringPubSubType()),
      [&](DataReader* reader) -> void
      {
        String msg;
        SampleInfo info;

        danube_prev = danube_now;
        danube_now = steady_clock::now();

        if (reader->take_next_sample(&msg, &info) == ReturnCode_t::RETCODE_OK)
        {
          if (info.valid_data)
          {
            printf("%s: Received String<%zu> from /danube, putting String<%zu> to /parana | <%ld μs>\n",
                   name.c_str(),
                   msg.data().size(),
                   parana_msg.data().size(),
                   duration_cast<microseconds>(danube_now - danube_prev).count());
           parana_writer->write(&parana_msg);
          }
        }
      }
    );

    // LOOP ============================================================================================================
    tigris_next = steady_clock::now();
    tigris_prev = steady_clock::now();
    tigris_now = steady_clock::now();

    ganges_next = steady_clock::now();
    ganges_prev = steady_clock::now();
    ganges_now = steady_clock::now();

    nile_next = steady_clock::now();
    nile_prev = steady_clock::now();
    nile_now = steady_clock::now();

    danube_next = steady_clock::now();
    danube_prev = steady_clock::now();
    danube_now = steady_clock::now();

    printf("%s: Starting loop\n", name.c_str());

    while (true)
    {
        std::this_thread::sleep_for(milliseconds(1000));
    }

    return 0;
}
