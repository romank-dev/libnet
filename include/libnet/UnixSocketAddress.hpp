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

#include <sys/un.h>
#include <string>

class UnixSocketAddress final
{
    public:

        UnixSocketAddress(const std::string& name, bool filesystem);

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
        operator const sockaddr_un&() const;

        /**
         * @brief Converts the `IpSocketAddress` object to a sockaddr_in structure.
         *
         * @return A sockaddr_in structure that holds the address and port.
         */
        operator sockaddr_un&();

    private:
        sockaddr_un make_addr(const std::string& name, bool filesystem);

    private:
        sockaddr_un         _addr;
};
