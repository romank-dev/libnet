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
#include <memory>
#include <netinet/in.h>

#include <libnet/UnixSocket.hpp>
#include <libnet/UnixServerSocket.hpp>
#include <libnet/UnixSocketAddress.hpp>

/**
 * @class UnixStreamSocket
 * @brief A class representing a Unix stream socket.
 */
class UnixStreamSocket final : public UnixSocket
{
	public:
       /**
         * @typedef sptr
         * @brief Shared pointer type for UnixStreamSocket.
         */
        using sptr = std::shared_ptr<UnixStreamSocket>;
        /**
         * @typedef uptr
         * @brief Unique pointer type for UnixStreamSocket.
         */
        using uptr = std::unique_ptr<UnixStreamSocket>;

        class ClientMode {};
        class ServerMode {};

        UnixStreamSocket(ClientMode, UnixSocketAddress address);
        UnixStreamSocket(ServerMode, UnixSocketAddress address, uint32_t timeout_ms);

        /**
        * @brief Destructor for UnixSocketAddress.
        */
        virtual ~UnixStreamSocket();

        /**
         * @brief Sends data through the socket.
         * @param data Pointer to the data to be sent.
         * @param size Size of the data to be sent.
         * @param num_sent Reference to store the number of bytes successfully sent.
         * @return True if the full size was sent, false if timed out. Throws on error.
         * @Note This method blocks until the data is sent or an timeout/error occurs.
         */
		bool send(const void* data, size_t size, size_t& num_sent);

		/**
         * @brief Sends data through the socket.
         * @param data Pointer to the data to be sent.
         * @param size Size of the data to be sent.
         * @Note This method blocks until the data is sent or an error/timeout occurs.
         * @throws PosixException on timeout and other errors.
         */
		void send(const void* data, size_t size);

		/**
         * @brief Receives data from the socket.
         * @param data Pointer to the buffer to store received data.
         * @param size Size of the buffer.
         * @param num_recvd Reference to store the number of bytes successfully received.
         * @return True if the full size was received, false if timed out. Throws on error.
         * @Note This method blocks until some data is received or an timeout/error occurs.
         */
		bool receive(void* data, size_t size, size_t& num_recvd);

		/**
         * @brief Receives data from the socket.
         * @param data Pointer to the buffer to store received data.
         * @param size Size of the buffer.
         * @Note This method blocks until all \ref size bytes are received or an error/timeout occurs.
         * @throws PosixException on timeout and other errors.
         */
		void receive(void* data, size_t size);

		/**
         * @brief Waits for the other end to close the connection.
         * @param timeout_ms Timeout in milliseconds to wait.
         * @return True if the connection was closed by the other end, false if timed out or data is available.
         */
		bool wait_for_close(uint32_t timeout_ms);

	private:
		friend uptr UnixServerSocket::accept_connection();
		friend uptr UnixServerSocket::accept_connection(uint32_t);

		explicit UnixStreamSocket(Handle&& sockfd); // take over existing sock fd given by UnixServerSocket::accept_connection()

};

