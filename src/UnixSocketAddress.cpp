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

UnixSocketAddress::UnixSocketAddress() : _addr{}
{
    _addr.sun_family = AF_UNIX;
}

UnixSocketAddress::UnixSocketAddress(const std::string& name, bool filesystem) : _addr(make_addr(name, filesystem))
{}

sockaddr_un UnixSocketAddress::make_addr(const std::string& name, bool filesystem)
{
    CHECK_THROW(!name.empty(), "Socket path cannot be empty");
    CHECK_THROW(name.length() < sizeof(sockaddr_un::sun_path) - 1 - int(filesystem), "Socket address name is too long")
    struct sockaddr_un addr {};
    addr.sun_family = AF_UNIX;
    // anonymous addresses not backed by a file will have '\0' as it's first letter.
    strncpy(addr.sun_path + (filesystem ? 0 : 1), name.c_str(), sizeof(addr.sun_path) - 1);
    return addr;
}

UnixSocketAddress UnixSocketAddress::make_temp_filesystem_path(const std::string& name)
{
    char path[sizeof(sockaddr_un::sun_path)];
    snprintf(path, sizeof(path), "%s", name.c_str());

    Handle file(mkstemp(path),"Failed to create temporary file");
    CHECK_THROW_POSIX(!unlink(path), "unlink failed"); // unlink immediately to avoid leaving a file on the filesystem
    return UnixSocketAddress(std::string(path), true);
}

UnixSocketAddress::operator std::string() const
{
    return is_filesystem() ? _addr.sun_path : _addr.sun_path+1;
}

UnixSocketAddress::operator const sockaddr_un&() const
{
    return _addr;
}

UnixSocketAddress::operator sockaddr_un&()
{
    return _addr;
}

size_t UnixSocketAddress::true_size() const
{
    return offsetof(struct sockaddr_un, sun_path) + strlen(_addr.sun_path+1) + 1;
}

bool UnixSocketAddress::is_filesystem() const
{
    return _addr.sun_path[0] != '\0';
}




