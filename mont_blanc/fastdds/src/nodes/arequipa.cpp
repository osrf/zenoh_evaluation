#include "Node.h"
#include "types/datatypes.h"
#include "types/datatypesPubSubTypes.h"

using namespace eprosima::fastdds::dds;

int main() {
    std::string name = "Arequipa";

    montblanc::Node node = montblanc::Node(name.c_str());
    node.init();

    DataReader* arkansas_reader = node.create_datareader(
      "/arkansas",
      static_cast<TypeSupport>(new StringPubSubType()),
      [&](DataReader* reader) -> void
      {
        String msg;
        SampleInfo info;

        if (reader->take_next_sample(&msg, &info) == ReturnCode_t::RETCODE_OK)
        {
            if (info.valid_data)
            {
              printf("%s: Received String from /arkansas\n", name.c_str());
            }
        }
      }
    );

    printf("%s: Starting loop\n", name.c_str());

    do
    {
      printf("\n\nPress [enter] to exit\n\n");
    } while (std::cin.get() != '\n');

    return 0;
}
