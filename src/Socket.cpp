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

#include <libnet/Socket.hpp>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <net/if.h>

using namespace std;

Socket::Socket(Type sock_type) : _sockfd(socket(AF_INET, sock_type == Type::TCP ? SOCK_STREAM : SOCK_DGRAM, 0))
{
    CHECK_THROW_POSIX(_sockfd >= 0, "socket() failed!");
}

Socket::Socket(int sock_fd) : _sockfd(sock_fd)
{
    CHECK_THROW_POSIX(sock_fd != -1, "Invalid socket!");
}

Socket::Socket(Type sock_type, uint16_t local_port, bool reuse_addr) : _sockfd(socket(AF_INET, sock_type == Type::TCP ? SOCK_STREAM : SOCK_DGRAM, 0))
{
    CHECK_THROW_POSIX(_sockfd >= 0, "socket() failed!");

    ExceptionGuard socket_free_guard([this]()
    {
        close(_sockfd);
    });

    if(reuse_addr)
    {
        CHECK_THROW_POSIX(setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse_addr, sizeof(int)) == 0, "setsockopt() failed");
    }

    bind_to_port(local_port);
}

void Socket::set_sock_opt(int level, int optname, void* optval, socklen_t optlen)
{
    CHECK_THROW_POSIX(!setsockopt(_sockfd, level, optname, optval, optlen), "setsockopt failed!");
}

int Socket::get_sock_opt(int level, int optname, void* optval, socklen_t* optlen)
{
	int res;
    CHECK_THROW_POSIX((res = getsockopt(_sockfd, level, optname, optval, optlen)) == 0, "getsockopt() failed");
    return res;
}

void Socket::bind_to_address(const sockaddr_in& addr)
{
    CHECK_THROW_POSIX(bind(_sockfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) != -1, "bind() failed!");
}

void Socket::bind_to_port(uint16_t port)
{
	struct sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind_to_address(addr);
}

Socket::~Socket()
{
    SAFE_DESTRUCTOR
    (
		int sock = _sockfd;
        _sockfd = 0;
        CHECK_THROW_POSIX(!close(sock), "close() failed");
    )
}

struct sockaddr_in Socket::addr_from_string(const string& ip, uint16_t port)
{
    struct sockaddr_in addr {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    CHECK_THROW(inet_aton(ip.c_str(), &addr.sin_addr) != 0, "invalid ip address given: %s", ip.c_str());

    return addr;
}

void Socket::bind_to_device(const string& iface_name)
{
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, iface_name.c_str(), sizeof(ifr.ifr_name));
    CHECK_THROW_POSIX(!setsockopt(_sockfd, SOL_SOCKET, SO_BINDTODEVICE, (void *)&ifr, sizeof(ifr)), "failed to bind socket to interface %s", iface_name.c_str());
}

void Socket::set_receive_timeout(int ms)
{
    struct timeval t;
    t.tv_sec = ms / 1000;
    t.tv_usec = (ms * 1000) % 1000000;
    CHECK_THROW_POSIX(setsockopt(_sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&t, sizeof(t)) == 0, "setsockopt() failed");
}

void Socket::set_send_timeout(int ms)
{
    struct timeval t;
    t.tv_sec = ms / 1000;
    t.tv_usec = (ms * 1000) % 1000000;
    CHECK_THROW_POSIX(setsockopt(_sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&t, sizeof(t)) == 0, "setsockopt() failed");
}





