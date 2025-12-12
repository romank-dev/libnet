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
#include <mutex>
#include <memory>
#include <netinet/in.h>
#include <libcommon/libcommon.hpp>
#include <libnet/IpSocketAddress.hpp>
#include <libnet/IpSocket.hpp>

class TcpSocket;

/**
 * @class TcpServerSocket
 * @brief A TCP server socket implementation for accepting incoming connections.
 *
 * The TcpServerSocket class provides functionality for creating and managing a TCP server socket.
 * It allows accepting incoming connections and supports optional timeout functionality.
 */
class TcpServerSocket final : public IpSocket
{
	public:
        /**
         * @brief Constructs a `TcpServerSocket` bound to the specified address and port.
         * @param bind_address The address and port on which the server will bind.
         * @param reuse_addr Whether to reuse the address if it is managed by the OS (SO_REUSEADDR flag).
         */
		TcpServerSocket(IpSocketAddress bind_address, bool reuse_addr);

		 /**
         * @brief Blocks and accepts an incoming connection.
         * @return A unique pointer to a `TcpSocket` object representing the accepted connection.
         * @throws PosixException on errors during the accept operation.
         */
		std::unique_ptr<TcpSocket> accept_connection();

		/**
         * @brief Blocks and accepts an incoming connection with a specified timeout.
         * @param timeout_ms Timeout in milliseconds to wait for a connection.
         * @return A unique pointer to a `TcpSocket` object representing the accepted connection, or null if timed out.
         * @throws PosixException on errors during the accept operation or timeout.
         */
		std::unique_ptr<TcpSocket> accept_connection(uint32_t timeout_ms);

};

