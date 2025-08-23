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
#include <libnet/UnixStreamSocket.hpp>
#include "logging.hpp"

using namespace std;

UnixStreamSocket::UnixStreamSocket(ServerMode, UnixSocketAddress address, uint32_t timeout_ms) :
    UnixSocket(Protocol::STREAM, address)
{
    TRACE_DBG("Created listening UNIX stream socket, fd: %d, on path: [%s]", int(_sockfd), string(address).c_str());
    CHECK_THROW_POSIX(listen(_sockfd, 1) != -1, "listen() failed!");

    struct sockaddr_in client_addr{};
    socklen_t client_addr_len = sizeof(client_addr);
    int client_sock_fd;
    CHECK_THROW(Utils::OS::wait_for_read_fd(_sockfd, timeout_ms), "Failed to accept a connection on time");
    CHECK_THROW_POSIX((client_sock_fd = accept(_sockfd, (struct sockaddr*)&client_addr, &client_addr_len)) != -1, "accept() failed!");

    TRACE_DBG("Listening Unix stream socket fd: %d accepted a client fd: %d", int(_sockfd), client_sock_fd);
    _sockfd = Handle(client_sock_fd, "Invalid client socket");
}

UnixStreamSocket::UnixStreamSocket(ClientMode, UnixSocketAddress address) :
    UnixSocket(Protocol::STREAM)
{
    TRACE_DBG("Created client UNIX stream socket, fd: %d", int(_sockfd));
    int connect_res = connect(_sockfd, (struct sockaddr*)&(const sockaddr_un&)address, address.true_size());
    CHECK_THROW_POSIX(connect_res == 0, "Failed to connect to [%s]", string(address).c_str());
    TRACE_DBG("Client UNIX stream socket (fd %d) connected to [%s]", int(_sockfd), string(address).c_str());
}

UnixStreamSocket::UnixStreamSocket(Handle&& sockfd) :
    UnixSocket(Protocol::STREAM, move(sockfd))
{
    TRACE_DBG("Created UNIX stream socket from server, fd: %d", int(_sockfd));
}

UnixStreamSocket::~UnixStreamSocket()
{
    SAFE_DESTRUCTOR
    (
        CHECK_THROW_POSIX(!shutdown(_sockfd, SHUT_RDWR), "shutdown() failed");
    )
}

bool UnixStreamSocket::send(const void* data, size_t size, size_t& num_sent)
{
    ssize_t sent = 0;
    num_sent = 0;
    while((sent = ::send(_sockfd, (char*)data+num_sent, size-num_sent, MSG_NOSIGNAL)) > 0 &&
            (num_sent += sent) < size)
    {}

    CHECK_THROW_POSIX(sent > 0 || errno == EAGAIN, "send() failed");
    return num_sent == size;
}

bool UnixStreamSocket::receive(void* data, size_t size, size_t& num_recvd)
{
    if(size == 0)
    {
        num_recvd = 0;
        return true;
    }

    ssize_t read;
    num_recvd = 0;
    while((read = ::read(_sockfd, (char*)data+num_recvd, size-num_recvd)) > 0 &&
            (num_recvd += read) < size)
    {}

    CHECK_THROW_POSIX(read >= 0 || errno == EAGAIN, "recv() failed");
    CHECK_THROW_POSIX(read != 0, "Connection is closed");

    return num_recvd == size;
}

void UnixStreamSocket::send(const void* data, size_t size)
{
    ssize_t sent = 0;
    size_t num_sent = 0;
    while((sent = ::send(_sockfd, (char*)data+num_sent, size-num_sent, MSG_NOSIGNAL)) > 0 &&
            (num_sent += sent) < size)
    {}

    CHECK_THROW_POSIX(sent != -1, "send() failed");
    CHECK_THROW_POSIX(num_sent == size, "timeout while sending data");
}

void UnixStreamSocket::receive(void* data, size_t size)
{
    if(size == 0)
        return;

    ssize_t read;
    size_t num_recvd = 0;
    while((read = ::read(_sockfd, (char*)data+num_recvd, size-num_recvd)) > 0 &&
            (num_recvd += read) < size)
    {}

    CHECK_THROW_POSIX(read >= 0 || errno == EAGAIN, "recv() failed");
    CHECK_THROW_POSIX(read != 0, "Connection is closed");
    CHECK_THROW_POSIX(num_recvd == size, "timeout while receiving data");
}

bool UnixStreamSocket::wait_for_close(uint32_t timeout_ms)
{
    char a_byte;
    Utils::OS::wait_for_read_fd(_sockfd, timeout_ms);
    int num_read = recv(_sockfd, &a_byte, 1, MSG_PEEK);
    CHECK_THROW_POSIX(num_read != -1 || errno == EAGAIN, "recv() failed");
    return num_read == 0;
}





