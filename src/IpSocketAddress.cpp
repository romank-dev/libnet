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

#include <libnet/IpSocketAddress.hpp>
#include <libcommon/libcommon.hpp>
#include <arpa/inet.h>

IpSocketAddress::IpSocketAddress(IpAddress address, uint16_t port) : _addr(make_addr(address, port))
{}

IpSocketAddress::IpSocketAddress(sockaddr_in addr) : _addr(addr)
{}

IpSocketAddress::IpSocketAddress() : _addr({})
{}

sockaddr_in IpSocketAddress::make_addr(IpAddress address, uint16_t port)
{
    sockaddr_in addr {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr = address;
    return addr;
}

IpAddress IpSocketAddress::address() const
{
    return _addr.sin_addr;
}

uint16_t IpSocketAddress::port() const
{
    return ntohs(_addr.sin_port);
}

IpSocketAddress::operator std::string() const
{
    return Utils::Data::format("%s:%d", inet_ntoa(_addr.sin_addr), ntohs(_addr.sin_port));
}

IpSocketAddress::operator const sockaddr_in&() const
{
    return _addr;
}

IpSocketAddress::operator sockaddr_in&()
{
    return _addr;
}






