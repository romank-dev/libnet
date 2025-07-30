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

TEST(LibNet, TestUnixStreamSocket)
{
    Thread server_thread([](Thread& thread)
    {
        UnixStreamSocket server(UnixStreamSocket::ServerMode{}, UnixSocketAddress("server_path", false), 1000);
        server.set_receive_timeout(1000);
        server.set_send_timeout(1000);
        char data[5];
        size_t recvd, sent;
        EXPECT_TRUE(server.receive(data, sizeof(data), recvd));
        EXPECT_EQ(string(data, 5), "Hello");
        EXPECT_TRUE(server.send(data, sizeof(data), sent));
        EXPECT_TRUE(server.wait_for_close(1000));
    });

    this_thread::sleep_for(chrono::milliseconds(100)); // let server start listening
    UnixStreamSocket client(UnixStreamSocket::ClientMode{}, UnixSocketAddress("server_path", false), 1000);
    client.set_receive_timeout(1000);
    client.set_send_timeout(1000);
    size_t recvd, sent;
    EXPECT_TRUE(client.send("Hello", 5, sent));
    char data[5];
    EXPECT_TRUE(client.receive(data, sizeof(data), recvd));
    EXPECT_EQ(string(data, 5), "Hello");
}

TEST(LibNet, TestUnixStreamSocketFilePath)
{
    Thread server_thread([](Thread& thread)
    {
        UnixStreamSocket server(UnixStreamSocket::ServerMode{}, UnixSocketAddress("/tmp/server_path", true), 1000);
        server.set_receive_timeout(1000);
        server.set_send_timeout(1000);
        char data[5];
        size_t recvd, sent;
        EXPECT_TRUE(server.receive(data, sizeof(data), recvd));
        EXPECT_EQ(string(data, 5), "Hello");
        EXPECT_TRUE(server.send(data, sizeof(data), sent));
        EXPECT_TRUE(server.wait_for_close(1000));
    });

    this_thread::sleep_for(chrono::milliseconds(100)); // let server start listening
    UnixStreamSocket client(UnixStreamSocket::ClientMode{}, UnixSocketAddress("/tmp/server_path", true), 1000);
    client.set_receive_timeout(1000);
    client.set_send_timeout(1000);
    size_t recvd, sent;
    EXPECT_TRUE(client.send("Hello", 5, sent));
    char data[5];
    EXPECT_TRUE(client.receive(data, sizeof(data), recvd));
    EXPECT_EQ(string(data, 5), "Hello");

    EXPECT_TRUE(File::is_accessible(FileSystemPath("/tmp/server_path")));
}

TEST(LibNet, TestUnixStreamServer)
{
    int num_clients = 10;
    auto addr = UnixSocketAddress("server_path", false);
    Thread server_thread([num_clients, addr](Thread& thread)
    {
        UnixServerSocket server(addr);
        for(int i = 0; i < num_clients; ++i)
        {
            auto conn = server.accept_connection(100);
            ASSERT_TRUE(conn != nullptr);
            conn->send("Hello", 5);
            char hello[5];
            size_t n;
            EXPECT_TRUE(conn->receive(hello, 5, n));
            EXPECT_STREQ(string(hello, 5).c_str(), "Hello");
        }
    });

    this_thread::sleep_for(chrono::milliseconds(50)); // let server start listening
    for(int i = 0; i < num_clients; i++)
    {
        UnixStreamSocket client(UnixStreamSocket::ClientMode{}, addr, 100);
        char hello[5];
        size_t n;
        EXPECT_TRUE(client.receive(hello, 5, n));
        EXPECT_STREQ(string(hello, 5).c_str(), "Hello");
        client.send("Hello", 5);
    }
}
