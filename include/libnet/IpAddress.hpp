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
#include <array>

/**
 * @class IpAddress
 * @brief Represents an IPv4 address.
 *
 * This class provides functionality to represent and manipulate IPv4 addresses.
 * It supports initialization from various formats, including string, integer,
 * and byte array representations. It also provides static instances for common
 * IPv4 addresses such as localhost, any address, and broadcast.
 */
class IpAddress final
{
    public:

        /**
         * @brief Represents the localhost address (127.0.0.1).
         */
        static IpAddress LocalHost;
        /**
         * @brief Represents the "any" address (0.0.0.0).
         */
        static IpAddress AnyAddress;
        /**
         * @brief Represents the broadcast address (255.255.255.255).
         */
        static IpAddress Broadcast;

        /**
        * @brief Constructs an IpAddress from a string representation.
        * @param ipv4address A string containing the IPv4 address (e.g., "192.168.1.1").
        */
        IpAddress(const std::string& ipv4address);

        /**
         * @brief Constructs an IpAddress from a 32-bit integer representation.
         * @param ipv4address A 32-bit integer containing the IPv4 address in network byte order.
         */
        IpAddress(uint32_t ipv4address);

        /**
         * @brief Constructs an IpAddress from an `in_addr` structure.
         * @param ipv4address An `in_addr` structure containing the IPv4 address.
         */
        IpAddress(in_addr ipv4address);

        /**
         * @brief Constructs an IpAddress from a byte array representation.
         * @param ipv4address A 4-byte array containing the IPv4 address in host byte order.
         */
        IpAddress(std::array<uint8_t, 4> ipv4address);

        /**
         * @brief Converts the IpAddress to a string representation.
         * @return A string containing the IPv4 address (e.g., "192.168.1.1").
         */
        operator std::string() const;

        /**
         * @brief Converts the IpAddress to an `in_addr` structure.
         * @return An `in_addr` structure containing the IPv4 address.
         */
        operator in_addr() const;

    private:
        static in_addr addr_from_str(const char* str);

    private:
        const in_addr     _addr;
};








