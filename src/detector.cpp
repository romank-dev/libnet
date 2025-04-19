/* Copyright 2025 Roman Kudinov. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <libcommon/libcommon.hpp>
#include <libnet/Detector.hpp>

using namespace std;


void usage()
{
    printf("USAGE: detector [-c|-s [ID]] \n"\
            "\t-c broadcast scan\n" \
            "\t-s [ID] listen with assigned ID\nThe port used is %u\n", DETECTOR_PORT);
}

int main(int argc, char** argv)
{
    try
    {
        if(argc < 2)
        {
            usage();
            return 0;
        }
        if(string(argv[1]) == "-c")
        {
            Detector d;

            auto ids = d.scan_and_collect(1000);

            for(const auto& id : ids)
            {
                printf("ID: %s IP: %s\n", id.first.c_str(), id.second.c_str());
            }

            printf("Done\n");
        }
        else if(string(argv[1]) == "-s" && argc == 3)
        {
            string id(argv[2]);

            Detector d;
            d.broadcast(id);
        }
        else usage();

    }
    catch(const exception& e)
    {
        printf("Caught exception: %s\n", e.what());
    }

	return 0;
}
