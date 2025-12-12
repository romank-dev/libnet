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

#include <libnet/IpSocket.hpp>
#include <libnet/IpSocketAddress.hpp>

/**
 * @class UdpSocket
 * @brief A class for handling UDP socket communication.
 *
 * The `UdpSocket` class provides functionality for sending and receiving UDP packets.
 * It is derived from the `IpSocket` class and offers additional methods specific to UDP communication.
 * The underlying socket is blocking. See \ref Socket for timeout configuration.
 */
class UdpSocket final : public IpSocket
{
	public:

        using sptr = std::shared_ptr<UdpSocket>;
        using uptr = std::unique_ptr<UdpSocket>;

        /**
         * @brief Constructs a `UdpSocket` and binds it to a specific local address.
         *
         * @param local_address The local address to bind the socket to.
         * @param reuse_addr Whether to allow address reuse (default is `false`).
         */
		UdpSocket(IpSocketAddress local_address, bool reuse_addr=false);

		 /**
         * @brief Default constructor for `UdpSocket`.
         *
         * Creates an unbound UDP socket.
         */
		UdpSocket();

		/**
         * @brief Sends data to a specified target address.
         *
         * @param data Pointer to the data to be sent.
         * @param size Size of the data in bytes.
         * @param target The target address to send the data to.
         */
		void send(const void* data, size_t size, IpSocketAddress target);

		/**
         * @brief Receives data from the socket.
         *
         * @param data Pointer to the buffer where received data will be stored.
         * @param size Size of the buffer in bytes.
         * @param packet_size Reference to store the size of the received packet.
         * @param from Reference to store the address of the sender.
         * @return `true` if a packet was received, `false` if the operation timed out.
         */
		bool receive(void* data, size_t size, uint16_t& packet_size, IpSocketAddress& from);

};
