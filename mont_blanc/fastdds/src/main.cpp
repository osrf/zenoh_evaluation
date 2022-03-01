#include "Node.h"
#include "types/datatypes.h"
#include "types/datatypesPubSubTypes.h"

using namespace eprosima::fastdds::dds;

int main() {
    std::string name = "WOW NODE";

    montblanc::Node node = montblanc::Node(name.c_str());
    node.init();

    DataWriter* wow_writer_a = node.create_datawriter("wow", static_cast<TypeSupport>(new HeaderPubSubType()));
    DataWriter* wow_writer_b = node.create_datawriter("wow", static_cast<TypeSupport>(new HeaderPubSubType()));
    DataReader* wow_reader = node.create_datareader(
      "wow",
      static_cast<TypeSupport>(new HeaderPubSubType()),
      [&](DataReader* reader) -> void
      {
        Header header_msg;
        SampleInfo info;

        if (reader->take_next_sample(&header_msg, &info) == ReturnCode_t::RETCODE_OK)
        {
            if (info.valid_data)
            {
              printf("%s: Received Header from wow: %s\n", name.c_str(), header_msg.frame_id().c_str());
            }
        }
      }
    );

    Header header_msg_a;
    Header header_msg_b;

    int msgsent = 0;
    char ch = 'y';
    do
    {
        if (ch == 'y')
        {
          header_msg_a.frame_id("AAA wow!" + std::to_string(msgsent));
          header_msg_b.frame_id("BBB wow!" + std::to_string(msgsent));

          wow_writer_a->write(&header_msg_a);
          wow_writer_b->write(&header_msg_b);
          ++msgsent;
          std::cout << "Sending sample, count=" << msgsent << ", send another sample?(y-yes,n-stop): ";
        }
        else if (ch == 'n')
        {
            std::cout << "Stopping execution " << std::endl;
            break;
        }
        else
        {
            std::cout << "Command " << ch << " not recognized, please enter \"y/n\":";
        }
    } while (std::cin >> ch);

    return 0;
}
