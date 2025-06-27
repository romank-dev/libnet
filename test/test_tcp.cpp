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

#include <libnet/TcpSocket.hpp>
#include "gtest/gtest.h"
using namespace std;



TEST(LibNet, TestClientServer)
{
    Thread server_thread([](Thread& thread)
    {
        TcpSocket server(IpSocketAddress(IpAddress::LocalHost, 12345), 1000);
        server.set_receive_timeout(1000);
        server.set_send_timeout(1000);
        char data[5];
        size_t recvd, sent;
        EXPECT_TRUE(server.receive(data, sizeof(data), recvd));
        EXPECT_EQ(string(data, 5), "Hello");
        EXPECT_TRUE(server.send(data, sizeof(data), sent));
    });

    TcpSocket client(TcpSocket::addr_from_string(IpAddress::LocalHost, 12345), 1000, false);
    client.set_receive_timeout(1000);
    client.set_send_timeout(1000);
    size_t recvd, sent;
    client.send("Hello", 5, sent);
    char data[5];
    client.receive(data, sizeof(data), recvd);
    EXPECT_EQ(string(data, 5), "Hello");
}
