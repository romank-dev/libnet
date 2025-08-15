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


#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <net/if.h>
#include <libnet/Socket.hpp>

using namespace std;

Socket::Socket(SockType type, Handle&& sockfd) :
        _sockfd(move(sockfd)),
        _type(type),
        _read_waitable(pollfd { _sockfd, POLLIN, 0 }),
        _write_waitable(pollfd { _sockfd, POLLOUT, 0 })
{}

Socket::SockType Socket::type() const
{
    return _type;
}

Waitable& Socket::get_read_waitable()
{
    return _read_waitable;
}

Waitable& Socket::get_write_waitable()
{
    return _write_waitable;
}

void Socket::set_sock_opt(int level, int optname, void* optval, socklen_t optlen)
{
    CHECK_THROW_POSIX(!setsockopt(_sockfd, level, optname, optval, optlen), "setsockopt failed!");
}

void Socket::get_sock_opt(int level, int optname, void* optval, socklen_t& optlen)
{
    CHECK_THROW_POSIX(!getsockopt(_sockfd, level, optname, optval, &optlen), "getsockopt() failed");
}

void Socket::set_receive_timeout(uint32_t ms)
{
    struct timeval t;
    t.tv_sec = ms / 1000;
    t.tv_usec = (ms * 1000) % 1000000;
    CHECK_THROW_POSIX(setsockopt(_sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&t, sizeof(t)) == 0, "setsockopt() failed");
}

void Socket::set_send_timeout(uint32_t ms)
{
    struct timeval t;
    t.tv_sec = ms / 1000;
    t.tv_usec = (ms * 1000) % 1000000;
    CHECK_THROW_POSIX(setsockopt(_sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&t, sizeof(t)) == 0, "setsockopt() failed");
}

