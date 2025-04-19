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

#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include <netinet/in.h>

#include <libnet/Socket.hpp>
#include <libnet/TcpServerSocket.hpp>

class TcpSocket final : public Socket
{
	public:
		explicit TcpSocket(const struct sockaddr_in& target);
		explicit TcpSocket(uint16_t local_port); // blocks until accepted the other endpoint

        TcpSocket(const std::string& ip, uint16_t port);

        virtual ~TcpSocket();

		void send(const void* data, uint32_t size);

		void receive(void* data, uint32_t size);

		// wait timeout_ms milliseconds for packet and write into data.
		// returns true if received packet, false if timed out.
		bool receive(void* data, uint32_t size, uint32_t timeout_ms);

	private:

		friend std::shared_ptr<TcpSocket> TcpServerSocket::accept_connection();
		friend std::shared_ptr<TcpSocket> TcpServerSocket::accept_connection(uint32_t);
		explicit TcpSocket(int sockfd); // take over existing socket given by TcpServerSocket::accept_connection()

};

typedef std::shared_ptr<TcpSocket> TcpSocketPtr;
