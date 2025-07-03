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



TEST(LibNet, TestClientServer)
{
    Thread server_thread([](Thread& thread)
    {
        TcpSocket server(IpSocketAddress(IpAddress::LocalHost, 12345), 1000, false);
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
    TcpSocket client(TcpSocket::addr_from_string(IpAddress::LocalHost, 12345), 1000);
    client.set_receive_timeout(1000);
    client.set_send_timeout(1000);
    size_t recvd, sent;
    EXPECT_TRUE(client.send("Hello", 5, sent));
    char data[5];
    EXPECT_TRUE(client.receive(data, sizeof(data), recvd));
    EXPECT_EQ(string(data, 5), "Hello");
}

TEST(LibNet, TestClientServerNoClientNoServer)
{
    Stopwatch sw;
    EXPECT_THROW(TcpSocket server(IpSocketAddress(IpAddress::LocalHost, 12345), 100, false);, Exception);
    EXPECT_GE(sw.get_ms(), 100);
    EXPECT_THROW(TcpSocket client(TcpSocket::addr_from_string(IpAddress::LocalHost, 12345), 100), Exception);
    EXPECT_LE(sw.get_ms(), 150); // shouldn't hang
}

TEST(LibNet, TestClientServerReuseServerSocket)
{
    Stopwatch sw;
    Thread client_thread([](Thread& thread)
    {
        this_thread::sleep_for(chrono::milliseconds(50)); // let server start listening
        TcpSocket client(IpSocketAddress(IpAddress::LocalHost, 12344), 50);
        client.send("hello", 5);
    });

    TcpSocket server_socket(IpSocketAddress(IpAddress::LocalHost, 12344), 100, true);
    EXPECT_LE(sw.get_ms(), 100);
    sw.reset();
    EXPECT_THROW(TcpSocket server_socket(IpSocketAddress(IpAddress::LocalHost, 12344), 100, true);, Exception);
    EXPECT_GE(sw.get_ms(), 100);
}


TEST(LibNet, TestClientServerNoReuseServerSocket)
{
    Stopwatch sw;
    Thread client_thread([](Thread& thread)
    {
        this_thread::sleep_for(chrono::milliseconds(50)); // let server start listening
        TcpSocket client(IpSocketAddress(IpAddress::LocalHost, 12343), 50);
        client.send("hello", 5);
    });

    TcpSocket server_socket(IpSocketAddress(IpAddress::LocalHost, 12343), 100, false);
    EXPECT_LE(sw.get_ms(), 100);
    sw.reset();
    EXPECT_THROW(TcpSocket server_socket(IpSocketAddress(IpAddress::LocalHost, 12343), 100, false);, Exception);
    EXPECT_LE(sw.get_ms(), 50);
}

TEST(LibNet, TestTcpServer)
{
    int num_clients = 10;
    Thread server_thread([num_clients](Thread& thread)
    {
        TcpServerSocket server(IpSocketAddress(IpAddress::LocalHost, 12343), true);
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
        TcpSocket client(IpSocketAddress(IpAddress::LocalHost, 12343), 100);
        char hello[5];
        size_t n;
        EXPECT_TRUE(client.receive(hello, 5, n));
        EXPECT_STREQ(string(hello, 5).c_str(), "Hello");
        client.send("Hello", 5);
    }
}

TEST(LibNet, TestTcpServerNoClients)
{
    TcpServerSocket server(IpSocketAddress(IpAddress::LocalHost, 12343), true);
    Stopwatch sw;
    auto conn = server.accept_connection(100);
    EXPECT_GE(sw.get_ms(), 100);
    EXPECT_EQ(conn, nullptr);
}






