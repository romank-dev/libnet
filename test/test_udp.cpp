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

#include <libnet/libnet.hpp>
#include "gtest/gtest.h"
using namespace std;

TEST(LibNet, TestUdpSocket)
{
    static constexpr int num_clients = 13;
    Thread server_thread([](Thread& thread)
    {
        UdpSocket server(IpSocketAddress(IpAddress::LocalHost, 12141), false);
        server.set_receive_timeout(1000);
        server.set_send_timeout(1000);
        for(int i = 0; i < num_clients; i++)
        {
            char data[5] {};
            uint16_t pkt_size;
            IpSocketAddress client;
            EXPECT_TRUE(server.receive(data, sizeof(data), pkt_size, client));
            EXPECT_EQ(string(data, 5), "Hello");
            server.send(data, sizeof(data), client);
        }
    });

    for(int i = 0; i < num_clients; i++)
    {
        UdpSocket client;
        client.send("Hello", 5, IpSocketAddress(IpAddress::LocalHost, 12141));
        client.set_receive_timeout(1000);
        client.set_send_timeout(1000);
        char data[5];
        uint16_t pkt_size;
        IpSocketAddress server;
        EXPECT_TRUE(client.receive(data, sizeof(data), pkt_size, server));
        EXPECT_EQ(string(data, 5), "Hello");
    }
}
