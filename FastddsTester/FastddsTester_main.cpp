// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Derived from DynamicHelloWorldExample

/**
 * @file FastddsTester_main.cpp
 *
 */

#include "FastddsTesterPublisher.h"
#include "FastddsTesterSubscriber.h"

#include <fastrtps/Domain.h>
#include <fastrtps/log/Log.h>

using namespace eprosima;
using namespace fastrtps;
using namespace rtps;

int type = 1;
int count = 50;
uint32_t size = 256; // default payload size in kB
long sleep = 500;

void usage()
{
    std::cout << "USAGE:" << std::endl;
    std::cout << "DynamicHelloWorld [role] [-c count] [-s sleep] [-k KB]" << std::endl;
    std::cout << "role: Can be 'publisher' (default) or 'subscriber'." << std::endl;
    std::cout << "count: Number of samples sent by a publisher. Without effect on subcribers." << std::endl;
    std::cout << "sleep: Time between samples sent by a publisher in millisecond. Without effect on subcribers." <<
        std::endl;
    std::cout << "KB: Size of payload" << std::endl;
}

bool parseArgs(
        int argc,
        char** argv)
{
    bool roleDefined = false;
    try
    {
        for (int i = 1; i < argc; ++i)
        {
            if (strcmp(argv[i], "publisher") == 0)
            {
                if (!roleDefined)
                {
                    type = 1;
                    roleDefined = true;
                }
                else
                {
                    std::cout << "role defined twice" << std::endl;
                    Log::Reset();
                    return false;
                }
            }
            else if (strcmp(argv[i], "subscriber") == 0)
            {
                if (!roleDefined)
                {
                    type = 2;
                    roleDefined = true;
                }
                else
                {
                    std::cout << "role defined twice" << std::endl;
                    Log::Reset();
                    return false;
                }
            }
            else if (strcmp(argv[i], "-c") == 0)
            {
                if (argc <= i + 1)
                {
                    std::cout << "count expected" << std::endl;
                    Log::Reset();
                    return false;
                }
                count = atoi(argv[++i]);
            }
            else if (strcmp(argv[i], "-s") == 0)
            {
                if (argc <= i + 1)
                {
                    std::cout << "sleep expected" << std::endl;
                    Log::Reset();
                    return false;
                }
                sleep = atoi(argv[++i]);
            }
            else if (strcmp(argv[i], "-k") == 0)
            {
                if (argc <= i + 1)
                {
                    std::cout << "KB expected" << std::endl;
                    Log::Reset();
                    return false;
                }
                size = atoi(argv[++i]);
            }
        }
    }
    catch (std::exception&)
    {
        usage();
        return false;
    }

    return true;
}

int main(
        int argc,
        char** argv)
{
    std::cout << "Starting " << std::endl;

    if (!parseArgs(argc, argv))
    {
        usage();
        return 0;
    }

    switch (type)
    {
        case 1:
        {
            FastddsTesterPublisher mypub;
            if (mypub.init(size))
            {
                mypub.run(count, sleep);
            }
            break;
        }
        case 2:
        {
            FastddsTesterSubscriber mysub;
            if (mysub.init(size))
            {
                mysub.run();
            }
            break;
        }
    }
    Domain::stopAll();
    Log::Reset();
    return 0;
}
