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

        /**
         * @brief Creates a temporary Unix socket address with a unique name.
         *
         * This function generates a unique name for the Unix socket address
         * by appending using mkstemp().
         *
         * @param name The base name for the Unix socket address.
         * @return A UnixSocketAddress object with a unique temporary path.
         */
        static UnixSocketAddress make_temp_filesystem_path(const std::string& name);

        /**
         * @brief Default constructor for `UnixSocketAddress`.
         *
         * Initializes an empty Unix abstract socket address.
         */
        UnixSocketAddress();

        /**
         * @brief Constructor for `UnixSocketAddress` with a given name and filesystem flag.
         *
         * @param name The name of the Unix socket address.
         * @param filesystem If true, the address is treated as a filesystem path. Otherwise, it addresses an abstract socket.
         */
        UnixSocketAddress(const std::string& name, bool filesystem);

        /**
         * @brief Converts the `UnixSocketAddress` object to a string representation.
         *
         * @return A string in the format "address:port".
         */
        operator std::string() const;

        /**
         * @brief Converts the `UnixSocketAddress` object to a sockaddr_in structure.
         *
         * @return A sockaddr_in structure that holds the address and port.
         */
        operator const sockaddr_un&() const;

        /**
         * @brief Gets the size of the socket address, to be passed to socket functions like `bind()`, `connect()`, etc.
         *
         * @return The size of the address within the sockaddr_un structure.
         */
        size_t true_size() const;

        /**
         * @brief Checks if the address is a file system path.
         *
         * @return True if the address is a file system path, false otherwise.
         */
        bool is_filesystem() const;

        /**
         * @brief Converts the `UnixSocketAddress` object to a sockaddr_un structure.
         *
         * @return A sockaddr_in structure that holds the address and port.
         */
        operator sockaddr_un&();

    private:
        sockaddr_un make_addr(const std::string& name, bool filesystem);

    private:
        sockaddr_un         _addr;
};
