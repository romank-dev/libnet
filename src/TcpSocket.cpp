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

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

TcpSocket::TcpSocket(uint16_t local_port) :
        Socket(Type::TCP, local_port)
{
    CHECK_THROW_ERRNO(listen(_sockfd, 1) != -1, "listen() failed!");

    struct sockaddr_in client_addr{};
    socklen_t client_addr_len = sizeof(client_addr);
    int client_sock_fd;
    CHECK_THROW_ERRNO((client_sock_fd = accept(_sockfd, (struct sockaddr*)&client_addr, &client_addr_len)) != -1, "accept() failed!");

    close(_sockfd);
    _sockfd = client_sock_fd;
}

TcpSocket::TcpSocket(const sockaddr_in& target) :
    Socket(Type::TCP)
{
    CHECK_THROW_ERRNO(connect(_sockfd, (struct sockaddr*)&target, sizeof(target)) != -1, "connect() failed!");
}

TcpSocket::TcpSocket(const string& ip, uint16_t port) :
    TcpSocket(Socket::addr_from_string(ip, port))
{}

TcpSocket::TcpSocket(int sockfd) :
    Socket(sockfd)
{}

TcpSocket::~TcpSocket()
{
    SAFE_DESTRUCTOR
    (
        CHECK_THROW_ERRNO(!shutdown(_sockfd, SHUT_RDWR), "shutdown() failed");
    )
}

void TcpSocket::send(const void* data, uint32_t size)
{
    uint total_sent = 0;
    int sent;
    while((sent = ::send(_sockfd, (char*)data+total_sent, size-total_sent, MSG_NOSIGNAL)) > 0 &&
            (total_sent += sent) < size)
    {}

    CHECK_THROW_ERRNO(total_sent == size, "send() failed");
}

void TcpSocket::receive(void* data, uint32_t size)
{
    int total_read = 0;
    int read;

    while((read = ::read(_sockfd, (char*)data+total_read, size-total_read)) > 0 && (total_read += read) < size)
    {}

    CHECK_THROW_ERRNO(total_read == size, "recv() failed");
}

bool TcpSocket::receive(void* data, uint32_t size, uint32_t timeout_ms)
{
    uint32_t current_timeout;
    socklen_t current_timeout_len = sizeof(current_timeout);
    CHECK_THROW_ERRNO(getsockopt(_sockfd, SOL_SOCKET, SO_RCVTIMEO, &current_timeout, &current_timeout_len) == 0, "getsockopt() failed");

    ScopeGuard restore_original_timeout([&]()
    {
        set_receive_timeout(current_timeout);
    });

    set_receive_timeout(timeout_ms);
    int total_read = 0;
    int read;
    while((read = ::read(_sockfd, (char*)data+total_read, size-total_read)) > 0 && (total_read += read) < size)
    {}

    CHECK_THROW_ERRNO(total_read == size || (errno == EAGAIN || errno == EWOULDBLOCK), "recv() failed");
    return total_read == size;
}








