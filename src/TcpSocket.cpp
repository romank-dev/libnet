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
#include <fcntl.h>
#include <libnet/TcpSocket.hpp>


using namespace std;

TcpSocket::TcpSocket(IpSocketAddress local_address, uint32_t timeout_ms, bool reuse_addr) :
    IpSocket(Protocol::TCP, local_address, reuse_addr)
{
    CHECK_THROW_POSIX(listen(_sockfd, 1) != -1, "listen() failed!");

    struct sockaddr_in client_addr{};
    socklen_t client_addr_len = sizeof(client_addr);
    int client_sock_fd;
    CHECK_THROW(Utils::OS::wait_for_read_fd(_sockfd, timeout_ms), "Failed to accept a connection on time");
    CHECK_THROW_POSIX((client_sock_fd = accept(_sockfd, (struct sockaddr*)&client_addr, &client_addr_len)) != -1, "accept() failed!");

    _sockfd = Handle(client_sock_fd, "Invalid client socket");
}

TcpSocket::TcpSocket(IpSocketAddress address, uint32_t timeout) :
    IpSocket(Protocol::TCP)
{
    Utils::OS::set_file_descriptor_blocking(_sockfd, false);
    int connect_res = connect(_sockfd, (struct sockaddr*)&(const sockaddr_in&)address, sizeof(sockaddr_in));
    ScopeGuard revert_blocking([this]()
    {
        Utils::OS::set_file_descriptor_blocking(_sockfd, true);
    });

    if(connect_res == 0)
        return;
    CHECK_THROW_POSIX(errno == EINPROGRESS, "connecting to %s failed", string(address).c_str());

    CHECK_THROW(Utils::OS::wait_for_write_fd(_sockfd, timeout), "Socket connect to %s timed out", string(address).c_str());

    int so_error = 0;
    socklen_t len = sizeof(so_error);
    get_sock_opt(SOL_SOCKET, SO_ERROR, &so_error, &len);
    errno = so_error;
    CHECK_THROW_POSIX(errno == 0, "Connection to %s failed", string(address).c_str());
}

TcpSocket::TcpSocket(Handle&& sockfd) :
    IpSocket(Protocol::TCP, move(sockfd))
{}

TcpSocket::~TcpSocket()
{
    SAFE_DESTRUCTOR
    (
        CHECK_THROW_POSIX(!shutdown(_sockfd, SHUT_RDWR), "shutdown() failed");
    )
}

bool TcpSocket::send(const void* data, size_t size, size_t& num_sent)
{
    ssize_t sent = 0;
    num_sent = 0;
    while((sent = ::send(_sockfd, (char*)data+num_sent, size-num_sent, MSG_NOSIGNAL)) > 0 &&
            (num_sent += sent) < size)
    {}

    CHECK_THROW_POSIX(sent > 0 || errno == EAGAIN, "send() failed");
    return num_sent == size;
}

bool TcpSocket::receive(void* data, size_t size, size_t& num_recvd)
{
    ssize_t read;
    num_recvd = 0;
    while((read = ::read(_sockfd, (char*)data+num_recvd, size-num_recvd)) > 0 &&
            (num_recvd += read) < size)
    {}

    CHECK_THROW_POSIX(read >= 0 || errno == EAGAIN, "recv() failed");
    CHECK_THROW_POSIX(read != 0, "Connection is closed");

    return num_recvd == size;
}

void TcpSocket::send(const void* data, size_t size)
{
    ssize_t sent = 0;
    size_t num_sent = 0;
    while((sent = ::send(_sockfd, (char*)data+num_sent, size-num_sent, MSG_NOSIGNAL)) > 0 &&
            (num_sent += sent) < size)
    {}

    CHECK_THROW_POSIX(sent > 0, "send() failed");
    CHECK_THROW_POSIX(num_sent == size, "timeout while sending data");
}

void TcpSocket::receive(void* data, size_t size)
{
    ssize_t read;
    size_t num_recvd = 0;
    while((read = ::read(_sockfd, (char*)data+num_recvd, size-num_recvd)) > 0 &&
            (num_recvd += read) < size)
    {}

    CHECK_THROW_POSIX(read >= 0, "recv() failed");
    CHECK_THROW_POSIX(read != 0, "Connection is closed");
    CHECK_THROW_POSIX(num_recvd == size, "timeout while receiving data");
}






