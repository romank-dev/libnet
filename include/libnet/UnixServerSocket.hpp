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

#include <libcommon/libcommon.hpp>
#include <libnet/UnixSocketAddress.hpp>
#include <libnet/UnixSocket.hpp>

class UnixStreamSocket;

/**
 * @class UnixServerSocket
 * @brief A class representing a Unix domain server socket for accepting incoming connections.
 *
 * This class provides functionality to create a server socket bound to a specific Unix domain address,
 * and allows accepting incoming client stream connections.
 */
class UnixServerSocket final : public UnixSocket
{
	public:

        /**
         * @brief Constructs a UnixServerSocket and binds it to the specified address.
         *
         * @param bind_address The Unix domain socket address to bind the server socket to.
         */
        explicit UnixServerSocket(UnixSocketAddress bind_address);

        /**
         * @brief Accepts an incoming connection and returns a stream socket for communication.
         *
         * @return A unique pointer to a `UnixStreamSocket` representing the accepted connection.
         */
		std::unique_ptr<UnixStreamSocket> accept_connection();

        /**
         * @brief Accepts an incoming connection with a timeout and returns a stream socket for communication.
         *
         * @param timeout_ms The timeout in milliseconds to wait for an incoming connection.
         * @return A unique pointer to a `UnixStreamSocket` representing the accepted connection.
         */
		std::unique_ptr<UnixStreamSocket> accept_connection(uint32_t timeout_ms);

};

