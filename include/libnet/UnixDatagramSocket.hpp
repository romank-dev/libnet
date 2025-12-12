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

#include <libnet/UnixSocket.hpp>
#include <libnet/UnixSocketAddress.hpp>

/**
 * @class UnixDatagramSocket
 * @brief A class representing a Unix Datagram Socket.
 *
 * This class provides functionality for sending and receiving datagram messages
 * over Unix domain sockets. The underlying socket is blocking. See \ref Socket for timeout configuration.
 */
class UnixDatagramSocket final : public UnixSocket
{
	public:

        /**
        * @typedef sptr
        * @brief Shared pointer type for UnixDatagramSocket.
        */
        using sptr = std::shared_ptr<UnixDatagramSocket>;
        /**
         * @typedef uptr
         * @brief Unique pointer type for UnixDatagramSocket.
         */
        using uptr = std::unique_ptr<UnixDatagramSocket>;

        /**
         * @brief Constructs a UnixDatagramSocket bound to a specific local address.
         * @param local_address The local address to bind the socket to.
         */
        explicit UnixDatagramSocket(UnixSocketAddress local_address);

        /**
         * @brief Constructs an unbound UnixDatagramSocket. It will not be able to
         * receive messages until bound to a local address.
         */
        UnixDatagramSocket();

        /**
         * @brief Sends data to a specified target address.
         * @param data Pointer to the data to be sent.
         * @param size Size of the data in bytes.
         * @param target The target address to send the data to.
         */
		void send(const void* data, size_t size, UnixSocketAddress target);

		 /**
         * @brief Receives data from the socket.
         * @param data Pointer to the buffer where the received data will be stored.
         * @param size Size of the buffer in bytes.
         * @param packet_size Reference to a variable where the size of the received packet will be stored.
         * @param from Reference to a UnixSocketAddress where the sender's address will be stored.
         * @return True if data was successfully received, false otherwise.
         */
		bool receive(void* data, size_t size, uint16_t& packet_size, UnixSocketAddress& from);

};
