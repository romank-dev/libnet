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
#include <libnet/UnixSocketAddress.hpp>
#include <libcommon/libcommon.hpp>

UnixSocketAddress::UnixSocketAddress(const std::string& name, bool filesystem) : _addr(make_addr(name, filesystem))
{}

sockaddr_un UnixSocketAddress::make_addr(const std::string& name, bool filesystem)
{
    struct sockaddr_un addr {};
    addr.sun_family = AF_UNIX;
    // anonymous addresses not backed by a file will have '\0' as it's first letter.
    strncpy(addr.sun_path + (filesystem ? 0 : 1), name.c_str(), name.length());
    return addr;
}


UnixSocketAddress::operator std::string() const
{
    return _addr.sun_path;
}

UnixSocketAddress::operator const sockaddr_un&() const
{
    return _addr;
}

UnixSocketAddress::operator sockaddr_un&()
{
    return _addr;
}






