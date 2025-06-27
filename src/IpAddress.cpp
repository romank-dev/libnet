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

#include <libnet/IpAddress.hpp>
#include <libcommon/libcommon.hpp>
#include <arpa/inet.h>

using namespace std;


IpAddress IpAddress::LocalHost(std::array<uint8_t, 4> {127,0,0,1});
IpAddress IpAddress::AnyAddress(0);
IpAddress IpAddress::Broadcast(0xffffffff);

IpAddress::IpAddress(const string& address) : _addr(addr_from_str(address.c_str()))
{}

IpAddress::IpAddress(uint32_t addr) : _addr{addr}
{}

IpAddress::IpAddress(std::array<uint8_t, 4> arr) :
    _addr
    {
        uint32_t(arr[0]) << 0  |
        uint32_t(arr[1]) << 8  |
        uint32_t(arr[2]) << 16 |
        uint32_t(arr[3]) << 24
    }
{}

IpAddress::IpAddress(in_addr ipv4address) : _addr(ipv4address)
{}

IpAddress::operator std::string() const
{
    return inet_ntoa(_addr);
}

IpAddress::operator in_addr() const
{
    return _addr;
}

in_addr IpAddress::addr_from_str(const char* str)
{
    in_addr addr;
    CHECK_THROW(inet_aton(str, &addr) != 0, "Invalid IPv4 address: %s", str);
    return addr;
}


