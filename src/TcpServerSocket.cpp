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
#include <libnet/IpSocket.hpp>
#include <libnet/TcpServerSocket.hpp>
#include <memory>

using namespace std;

TcpServerSocket::TcpServerSocket(IpSocketAddress bind_addr, bool reuse_addr) :
    IpSocket(Protocol::TCP, bind_addr, reuse_addr)
{
    CHECK_THROW_POSIX(listen(_sockfd, 1) != -1, "listen() failed!");
}

TcpSocket::uptr TcpServerSocket::accept_connection()
{
    struct sockaddr_in client;
    socklen_t client_len = sizeof(client);
    int client_fd = accept(_sockfd, (struct sockaddr*)&client, &client_len);
    CHECK_THROW_POSIX(client_fd > 0, "accept() failed!");
    return TcpSocket::uptr(new TcpSocket(Handle(client_fd)));
}

TcpSocket::uptr TcpServerSocket::accept_connection(uint32_t timeout_ms)
{
    struct sockaddr_in client;
    socklen_t client_len = sizeof(client);
    if(!Utils::OS::wait_for_read_fd(_sockfd, timeout_ms))
    	return nullptr;
    int client_fd = accept(_sockfd, (struct sockaddr*)&client, &client_len);
    CHECK_THROW_POSIX(client_fd > 0, "accept() failed!");
    return TcpSocket::uptr(new TcpSocket(Handle(client_fd)));
}

