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

#include <netinet/in.h>
#include <string>
#include <libnet/IpAddress.hpp>

/**
 * @class IpSocketAddress
 * @brief Represents an IPv4 address combined with a port number.
 */
class IpSocketAddress final
{
    public:
        /**
         * @brief Constructs an `IpSocketAddress` object.
         *
         * @param address The IPv4 address.
         * @param port The port number in host byte order.
         */
        IpSocketAddress(IpAddress address, uint16_t port);

        /** @brief Constructs an `IpSocketAddress` object from a sockaddr_in structure.
         * @param addr The sockaddr_in structure containing the IPv4 address and port.
         */
        IpSocketAddress(sockaddr_in addr);

        /** @brief Default constructor for `IpSocketAddress`.
         *         Will be initialized to 0.0.0.0:0
         */
        IpSocketAddress();

        /**
         * @brief Retrieves the IPv4 address.
         *
         * @return The `IpAddress` object representing the IPv4 address.
         */
        IpAddress address() const;

        /**
        * @brief Retrieves the port number in host byte order.
        *
        * @return The port number as a 16-bit unsigned integer.
        */
        uint16_t port() const;

        /**
         * @brief Converts the `IpSocketAddress` object to a string representation.
         *
         * @return A string in the format "address:port".
         */
        operator std::string() const;

        /**
         * @brief Converts the `IpSocketAddress` object to a sockaddr_in structure.
         *
         * @return A sockaddr_in structure that holds the address and port.
         */
        operator const sockaddr_in&() const;

        /**
         * @brief Converts the `IpSocketAddress` object to a sockaddr_in structure.
         *
         * @return A sockaddr_in structure that holds the address and port.
         */
        operator sockaddr_in&();

    private:
        sockaddr_in make_addr(IpAddress address, uint16_t port);

    private:
        sockaddr_in         _addr;
};
