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

#include <libnet/UnixStreamSocket.hpp>
#include <libnet/UnixSocket.hpp>
#include <libnet/UnixServerSocket.hpp>
#include <memory>
#include "logging.hpp"

using namespace std;

UnixServerSocket::UnixServerSocket(UnixSocketAddress bind_addr) :
        UnixSocket(Protocol::STREAM, bind_addr)
{
    TRACE_DBG("Created UNIX stream server socket, fd: %d", int(_sockfd));
    CHECK_THROW_POSIX(listen(_sockfd, 1) != -1, "listen() failed!");
}

UnixStreamSocket::uptr UnixServerSocket::accept_connection()
{
    struct sockaddr_in client;
    socklen_t client_len = sizeof(client);
    int client_fd = accept(_sockfd, (struct sockaddr*)&client, &client_len);
    CHECK_THROW_POSIX(client_fd > 0, "accept() failed!");
    return UnixStreamSocket::uptr(new UnixStreamSocket(Handle(client_fd)));
}

UnixStreamSocket::uptr UnixServerSocket::accept_connection(uint32_t timeout_ms)
{
    struct sockaddr_in client;
    socklen_t client_len = sizeof(client);
    if(!Utils::OS::wait_for_read_fd(_sockfd, timeout_ms))
    	return nullptr;
    int client_fd = accept(_sockfd, (struct sockaddr*)&client, &client_len);
    CHECK_THROW_POSIX(client_fd > 0, "accept() failed!");
    return UnixStreamSocket::uptr(new UnixStreamSocket(Handle(client_fd)));
}

