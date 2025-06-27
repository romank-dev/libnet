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

#include <libnet/TcpSocket.hpp>
#include <libnet/TcpServerSocket.hpp>

TcpServerSocket::TcpServerSocket(uint16_t local_port) :
    _sockfd(socket(AF_INET, SOCK_STREAM, 0), "Failed to create server socket")
{
    ExceptionGuard socket_free_guard([this]()
    {
        close(_sockfd);
    });

    int reuse = 1;
    CHECK_THROW_POSIX(setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == 0, "setsockopt() failed");

    struct sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(local_port);
    addr.sin_addr.s_addr = INADDR_ANY;
    CHECK_THROW_POSIX(bind(_sockfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) != -1, "bind() failed!");

    CHECK_THROW_POSIX(listen(_sockfd, 1) != -1, "listen() failed!");
}

TcpServerSocket::~TcpServerSocket()
{
    SAFE_DESTRUCTOR
    (
        close(_sockfd);
    )
}

TcpSocket::sptr TcpServerSocket::accept_connection()
{
    struct sockaddr_in client;
    socklen_t client_len = sizeof(client);
    int client_fd = accept(_sockfd, (struct sockaddr*)&client, &client_len);
    CHECK_THROW_POSIX(client_fd > 0, "accept() failed!");
    return std::shared_ptr<TcpSocket>(new TcpSocket(Handle(client_fd)));
}

TcpSocket::sptr TcpServerSocket::accept_connection(uint32_t timeout_ms)
{
    struct sockaddr_in client;
    socklen_t client_len = sizeof(client);
    if(!Utils::OS::wait_for_read_fd(_sockfd, timeout_ms))
    	return nullptr;
    int client_fd = accept(_sockfd, (struct sockaddr*)&client, &client_len);
    CHECK_THROW_POSIX(client_fd > 0, "accept() failed!");
    return std::shared_ptr<TcpSocket>(new TcpSocket(client_fd));
}

