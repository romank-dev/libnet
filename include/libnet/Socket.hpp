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

/**
 * @class Socket
 * @brief Base class for socket implementations.
 *
 * The `Socket` class provides a common interface for managing socket operations.
 * It supports setting and retrieving socket options, as well as configuring send and receive timeouts.
 * This class is abstract and cannot be instantiated directly.
 */
class Socket : NonCopyable
{
    public:
        /**
         * @typedef sptr
         * @brief Shared pointer type for `Socket`.
         */
        using sptr = std::shared_ptr<Socket>;

        /**
        * @typedef uptr
        * @brief Unique pointer type for `Socket`.
        */
        using uptr = std::unique_ptr<Socket>;

        /**
         * @enum SockType
         * @brief Defines the type of socket.
         *
         * - `IPv4`: Represents an IPv4 socket.
         * - `UNIX`: Represents a UNIX domain socket.
         */
        enum class SockType
        {
            IPv4,
            UNIX
        };

    public:

        /**
         * @brief Virtual destructor for `Socket`.
         *
         * Ensures proper cleanup of resources in derived classes.
         */
        virtual ~Socket() = default;

        /**
         * @brief Retrieves the type of the socket.
         * @return The socket type (`SockType`).
         */
        SockType type() const;

        /**
         * @brief Sets the send timeout for the socket.
         * @param ms Timeout in milliseconds.
         */
        void set_send_timeout(uint32_t ms);

        /**
         * @brief Sets the receive timeout for the socket.
         * @param ms Timeout in milliseconds.
         */
        void set_receive_timeout(uint32_t ms);

        /**
        * @brief Sets a socket option.
        * @param level The protocol level at which the option resides.
        * @param optname The name of the option.
        * @param optval Pointer to store the option value.
        * @param optlen the size of the optval buffer.
        */
        void set_sock_opt(int level, int optname, void* optval, socklen_t optlen);

        /**
         * @brief Retrieves a socket option.
         * @param level The protocol level at which the option resides.
         * @param optname The name of the option.
         * @param optval Pointer to store the option value.
         * @param optlen Pointer to the size of the given optval buffer, \
         *               which is modified with the resulting optval buffer.
         */
        void get_sock_opt(int level, int optname, void* optval, socklen_t& optlen);


    protected:
        explicit Socket(SockType type, Handle&& sockfd);

    protected:
        Handle              _sockfd;
        const SockType      _type;
};




