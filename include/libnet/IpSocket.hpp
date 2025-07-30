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
#include <libnet/Socket.hpp>
#include <libnet/IpSocketAddress.hpp>

/**
 * @class IpSocket
 * @brief Represents an IPv4 socket.
 *
 * The `IpSocket` class provides common functionality for IPv4 sockets (e.g. bind)
 * and is the base class of TcpSocket and UdpSocket.
 */
class IpSocket : public Socket
{
    public:

        /**
         * @brief Shared pointer type alias for `IpSocket`.
         */
        using sptr = std::shared_ptr<IpSocket>;
        /**
         * @brief Unique pointer type alias for `IpSocket`.
         */
        using uptr = std::unique_ptr<IpSocket>;

        /**
         * @enum Protocol
         * @brief Enum representing the supported socket protocols.
         */
        enum class Protocol
        {
            TCP,
            UDP
        };

    public:

        /**
         * @brief Binds the socket to a specific address and port.
         *
         * @param addr The `sockaddr_in` structure representing the address to bind to.
         */
		void bind_to_address(IpSocketAddress addr);

		 /**
         * @brief Binds the socket to a specific port.
         *        The address used is 0.0.0.0
         *
         * @param port The port number to bind the socket to.
         */
		void bind_to_port(uint16_t port);

		/**
         * @brief Binds the socket to a specific network device.
         *
         * @param iface_name The name of the network interface to bind to.
         */
		void bind_to_device(const std::string& iface_name);

		/**
		 * @brief Checks if this socket called bind() on the address being used.
		 *
		 * @return True if the socket owns the address, false otherwise.
		 */
        bool owns_address() const;

		 /**
         * @brief Retrieves the protocol type of the socket.
         *
         * @return The protocol type (`Protocol::TCP` or `Protocol::UDP`).
         */
		Protocol protocol() const;

    protected:
        explicit IpSocket(Protocol sock_type);
        IpSocket(Protocol sock_type, Handle&& sock_fd);
        IpSocket(Protocol sock_type, IpSocketAddress local_address, bool reuse_addr); // socket() + bind()

    protected:
        const Protocol        _protocol;

    private:
        bool                  _owns_address;

};










