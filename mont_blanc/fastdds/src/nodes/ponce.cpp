#include <iostream>
#include <chrono>
#include <thread>

#include "Node.h"
#include "utils.hpp"

#include "types/datatypes.h"
#include "types/datatypesPubSubTypes.h"


using namespace eprosima::fastdds::dds;

int main() {
    std::string name = "Ponce";

    montblanc::Node node = montblanc::Node(name.c_str());
    node.init();

    using namespace std::chrono;

    auto tagus_next = steady_clock::now();
    auto tagus_prev = steady_clock::now();
    auto tagus_now = steady_clock::now();

    auto danube_next = steady_clock::now();
    auto danube_prev = steady_clock::now();
    auto danube_now = steady_clock::now();

    auto missouri_next = steady_clock::now();
    auto missouri_prev = steady_clock::now();
    auto missouri_now = steady_clock::now();

    auto brazos_next = steady_clock::now();
    auto brazos_prev = steady_clock::now();
    auto brazos_now = steady_clock::now();

    auto yamuna_next = steady_clock::now();
    auto yamuna_prev = steady_clock::now();
    auto yamuna_now = steady_clock::now();

    auto godavari_next = steady_clock::now();
    auto godavari_prev = steady_clock::now();
    auto godavari_now = steady_clock::now();

    auto loire_next = steady_clock::now();
    auto loire_prev = steady_clock::now();
    auto loire_now = steady_clock::now();

    auto ohio_next = steady_clock::now();
    auto ohio_prev = steady_clock::now();
    auto ohio_now = steady_clock::now();

    auto volga_next = steady_clock::now();
    auto volga_prev = steady_clock::now();
    auto volga_now = steady_clock::now();

    // PUB =============================================================================================================
    DataWriter* congo_writer = node.create_datawriter("/congo", static_cast<TypeSupport>(new TwistPubSubType()));
    Twist congo_msg;

    DataWriter* mekong_writer = node.create_datawriter(
        "/mekong", static_cast<TypeSupport>(new TwistWithCovarianceStampedPubSubType()));
    TwistWithCovarianceStamped mekong_msg;

    // RANDOMIZE =======================================================================================================
    printf("%s: Data generation started\n", name.c_str());

    congo_msg = montblanc::random_twist();
    mekong_msg = montblanc::random_twistwithcovariancestamped();

    printf("%s: Data generation done\n\n", name.c_str());

    // SUB =============================================================================================================
    DataReader* tagus_reader = node.create_datareader(
      "/tagus",
      static_cast<TypeSupport>(new PosePubSubType()),
      [&](DataReader* reader) -> void
      {
        Pose msg;
        SampleInfo info;

        tagus_prev = tagus_now;
        tagus_now = steady_clock::now();

        if (reader->take_next_sample(&msg, &info) == ReturnCode_t::RETCODE_OK)
        {
          if (info.valid_data)
          {
            printf("%s: Received Pose from /tagus | <%ld μs>\n",
                   name.c_str(),
                   duration_cast<microseconds>(tagus_now - tagus_prev).count());
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
            printf("%s: Received String<%zu> from /danube | <%ld μs>\n",
                   name.c_str(),
                   msg.data().size(),
                   duration_cast<microseconds>(danube_now - danube_prev).count());
          }
        }
      }
    );

    DataReader* missouri_reader = node.create_datareader(
      "/missouri",
      static_cast<TypeSupport>(new ImagePubSubType()),
      [&](DataReader* reader) -> void
      {
        Image msg;
        SampleInfo info;

        missouri_prev = missouri_now;
        missouri_now = steady_clock::now();

        if (reader->take_next_sample(&msg, &info) == ReturnCode_t::RETCODE_OK)
        {
          if (info.valid_data)
          {
            printf("%s: Received Image<%zu> from /missouri | <%ld μs>\n",
                   name.c_str(),
                   msg.data().size(),
                   duration_cast<microseconds>(missouri_now - missouri_prev).count());
          }
        }
      }
    );

    DataReader* brazos_reader = node.create_datareader(
      "/brazos",
      static_cast<TypeSupport>(new PointCloud2PubSubType()),
      [&](DataReader* reader) -> void
      {
        PointCloud2 msg;
        SampleInfo info;

        brazos_prev = brazos_now;
        brazos_now = steady_clock::now();

        if (reader->take_next_sample(&msg, &info) == ReturnCode_t::RETCODE_OK)
        {
          if (info.valid_data)
          {
            printf("%s: Received PointCloud2<%zu> from /brazos, putting Twist to /congo, "
                   "putting TwistWithCovarianceStamped<%zu> to /mekong | <%ld μs>\n",
                   name.c_str(),
                   msg.data().size(),
                   mekong_msg.twist().covariance().size(),
                   duration_cast<microseconds>(brazos_now - brazos_prev).count());
            congo_writer->write(&congo_msg); // ========================================================================
            mekong_writer->write(&mekong_msg); // ======================================================================
          }
        }
      }
    );

    DataReader* yamuna_reader = node.create_datareader(
      "/yamuna",
      static_cast<TypeSupport>(new Vector3PubSubType()),
      [&](DataReader* reader) -> void
      {
        Vector3 msg;
        SampleInfo info;

        yamuna_prev = yamuna_now;
        yamuna_now = steady_clock::now();

        if (reader->take_next_sample(&msg, &info) == ReturnCode_t::RETCODE_OK)
        {
          if (info.valid_data)
          {
            printf("%s: Received Vector3 from /yamuna | <%ld μs>\n",
                   name.c_str(),
                   duration_cast<microseconds>(yamuna_now - yamuna_prev).count());
          }
        }
      }
    );

    DataReader* godavari_reader = node.create_datareader(
      "/godavari",
      static_cast<TypeSupport>(new LaserScanPubSubType()),
      [&](DataReader* reader) -> void
      {
        LaserScan msg;
        SampleInfo info;

        godavari_prev = godavari_now;
        godavari_now = steady_clock::now();

        if (reader->take_next_sample(&msg, &info) == ReturnCode_t::RETCODE_OK)
        {
          if (info.valid_data)
          {
            printf("%s: Received LaserScan<%zu, %zu> from /godavari | <%ld μs>\n",
                   name.c_str(),
                   msg.ranges().size(),
                   msg.intensities().size(),
                   duration_cast<microseconds>(godavari_now - godavari_prev).count());
          }
        }
      }
    );

    DataReader* loire_reader = node.create_datareader(
      "/loire",
      static_cast<TypeSupport>(new PointCloud2PubSubType()),
      [&](DataReader* reader) -> void
      {
        PointCloud2 msg;
        SampleInfo info;

        loire_prev = loire_now;
        loire_now = steady_clock::now();

        if (reader->take_next_sample(&msg, &info) == ReturnCode_t::RETCODE_OK)
        {
          if (info.valid_data)
          {
            printf("%s: Received PointCloud2<%zu> from /loire | <%ld μs>\n",
                   name.c_str(),
                   msg.data().size(),
                   duration_cast<microseconds>(loire_now - loire_prev).count());
          }
        }
      }
    );

    DataReader* ohio_reader = node.create_datareader(
      "/ohio",
      static_cast<TypeSupport>(new Float32PubSubType()),
      [&](DataReader* reader) -> void
      {
        Float32 msg;
        SampleInfo info;

        ohio_prev = ohio_now;
        ohio_now = steady_clock::now();

        if (reader->take_next_sample(&msg, &info) == ReturnCode_t::RETCODE_OK)
        {
          if (info.valid_data)
          {
            printf("%s: Received Float32 from /ohio | <%ld μs>\n",
                   name.c_str(),
                   duration_cast<microseconds>(ohio_now - ohio_prev).count());
          }
        }
      }
    );

    DataReader* volga_reader = node.create_datareader(
      "/volga",
      static_cast<TypeSupport>(new Float64PubSubType()),
      [&](DataReader* reader) -> void
      {
        Float64 msg;
        SampleInfo info;

        volga_prev = volga_now;
        volga_now = steady_clock::now();

        if (reader->take_next_sample(&msg, &info) == ReturnCode_t::RETCODE_OK)
        {
          if (info.valid_data)
          {
            printf("%s: Received Float64 from /volga | <%ld μs>\n",
                   name.c_str(),
                   duration_cast<microseconds>(volga_now - volga_prev).count());
          }
        }
      }
    );

    // LOOP ============================================================================================================
    tagus_next = steady_clock::now();
    tagus_prev = steady_clock::now();
    tagus_now = steady_clock::now();

    danube_next = steady_clock::now();
    danube_prev = steady_clock::now();
    danube_now = steady_clock::now();

    missouri_next = steady_clock::now();
    missouri_prev = steady_clock::now();
    missouri_now = steady_clock::now();

    brazos_next = steady_clock::now();
    brazos_prev = steady_clock::now();
    brazos_now = steady_clock::now();

    yamuna_next = steady_clock::now();
    yamuna_prev = steady_clock::now();
    yamuna_now = steady_clock::now();

    godavari_next = steady_clock::now();
    godavari_prev = steady_clock::now();
    godavari_now = steady_clock::now();

    loire_next = steady_clock::now();
    loire_prev = steady_clock::now();
    loire_now = steady_clock::now();

    ohio_next = steady_clock::now();
    ohio_prev = steady_clock::now();
    ohio_now = steady_clock::now();

    volga_next = steady_clock::now();
    volga_prev = steady_clock::now();
    volga_now = steady_clock::now();

    printf("%s: Starting loop\n", name.c_str());

    while (true)
    {
        std::this_thread::sleep_for(milliseconds(1000));
    }

    return 0;
}
