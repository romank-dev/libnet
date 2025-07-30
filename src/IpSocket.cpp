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

#include <libnet/IpSocket.hpp>

using namespace std;

IpSocket::IpSocket(Protocol protocol) :
    Socket(SockType::IPv4, Handle(socket(AF_INET, protocol == Protocol::TCP ? SOCK_STREAM : SOCK_DGRAM, 0), "Failed to create socket")),
    _protocol(protocol)
{}

IpSocket::IpSocket(Protocol sock_type, Handle&& sock_fd) :
    Socket(SockType::IPv4, move(sock_fd)),
    _protocol(sock_type)
{
    CHECK_THROW_POSIX(_sockfd != -1, "Invalid socket!");
}

IpSocket::IpSocket(Protocol protocol, IpSocketAddress local_address, bool reuse_addr) :
    Socket(SockType::IPv4, Handle(socket(AF_INET, protocol == Protocol::TCP ? SOCK_STREAM : SOCK_DGRAM, 0), "Failed to create socket")),
    _protocol(protocol)
{
    if(reuse_addr)
    {
        CHECK_THROW_POSIX(setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse_addr, sizeof(int)) == 0, "setsockopt() failed");
    }

    bind_to_port(local_address.port());
}

void IpSocket::bind_to_address(IpSocketAddress addr)
{
    CHECK_THROW_POSIX(::bind(_sockfd, (struct sockaddr*)&(addr.operator sockaddr_in &()), sizeof(struct sockaddr_in)) != -1, "bind() failed!");
}

void IpSocket::bind_to_port(uint16_t port)
{
	struct sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind_to_address(addr);
}

void IpSocket::bind_to_device(const string& iface_name)
{
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, iface_name.c_str(), sizeof(ifr.ifr_name));
    CHECK_THROW_POSIX(!setsockopt(_sockfd, SOL_SOCKET, SO_BINDTODEVICE, (void *)&ifr, sizeof(ifr)), "failed to bind socket to interface %s", iface_name.c_str());
}

IpSocket::Protocol IpSocket::protocol() const
{
    return _protocol;
}






