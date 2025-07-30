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
#include <libnet/UnixSocketAddress.hpp>

/**
 * @class UnixSocket
 * @brief Represents a Unix domain socket.
 *
 * The UnixSocket class provides an abstraction for Unix domain sockets,
 * supporting both STREAM and DGRAM protocols.
 */
class UnixSocket : public Socket
{
    public:
        /**
         * @brief Shared pointer type alias for UnixSocket.
         */
        using sptr = std::shared_ptr<UnixSocket>;
        /**
        * @brief Unique pointer type alias for UnixSocket.
        */
        using uptr = std::unique_ptr<UnixSocket>;

        /**
        * @enum Protocol
        * @brief Defines the protocol type for the UnixSocket.
        */
        enum class Protocol
        {
            STREAM,
            DGRAM
        };

    public:

        virtual ~UnixSocket();

        /**
         * @brief binds the socket to a specified Unix domain socket address.
         * @param address The UnixSocketAddress to bind to.
         */
		void bind_to_path(UnixSocketAddress address);

		/**
		 * @brief Retrieves the protocol type of the UnixSocket.
		 * @return The protocol type as an enum value.
		 */
		Protocol protocol() const;

		/**
         * @brief Returns true if the socket is bound on a UnixSocketAddress.
         * @return true if the socket is bound on a UnixSocketAddress.
         */
		bool owns_address() const;

    protected:
        explicit UnixSocket(Protocol sock_type);
        UnixSocket(Protocol sock_type, Handle&& sock_fd);
        UnixSocket(Protocol sock_type, UnixSocketAddress address);

    protected:
        const Protocol        _protocol;

    private:
        bool                  _owns_address;

};
