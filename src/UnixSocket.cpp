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

#include <libnet/UnixSocket.hpp>

using namespace std;

UnixSocket::UnixSocket(Protocol protocol) :
    Socket(SockType::UNIX, Handle(socket(AF_UNIX, protocol == Protocol::STREAM ? SOCK_STREAM : SOCK_DGRAM, 0), "Failed to create socket")),
    _protocol(protocol)
{}

UnixSocket::UnixSocket(Protocol sock_type, Handle&& sock_fd) :
    Socket(SockType::UNIX, move(sock_fd)),
    _protocol(sock_type)
{
    CHECK_THROW_POSIX(_sockfd != -1, "Invalid socket!");
}

UnixSocket::UnixSocket(Protocol protocol, UnixSocketAddress address) :
    Socket(SockType::UNIX, Handle(socket(AF_UNIX, protocol == Protocol::STREAM ? SOCK_STREAM : SOCK_DGRAM, 0), "Failed to create socket")),
    _protocol(protocol)
{
    bind_to_path(address);
}

void UnixSocket::bind_to_path(UnixSocketAddress addr)
{
    CHECK_THROW_POSIX(!::bind(_sockfd, (struct sockaddr*)&addr, sizeof(addr)), "bind() failed");
}

UnixSocket::Protocol UnixSocket::protocol() const
{
    return _protocol;
}
