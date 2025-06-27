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

#include <libnet/IpSocket.hpp>
#include <libnet/TcpServerSocket.hpp>
#include <libnet/IpSocketAddress.hpp>

/**
 * @class TcpSocket
 * @brief A RAII-style TCP connected socket implementation.
 *
 * The TcpSocket connects or accepts a connection during construction, behaving as a connected socket
 * during it's lifetime. The underlying socket is blocking.
 */
class TcpSocket final : public IpSocket
{
	public:
       /**
         * @typedef sptr
         * @brief Shared pointer type for TcpSocket.
         */
        using sptr = std::shared_ptr<TcpSocket>;
        /**
         * @typedef uptr
         * @brief Unique pointer type for TcpSocket.
         */
        using uptr = std::unique_ptr<TcpSocket>;

        /**
         * @brief Constructs a TcpSocket and connects to the specified address.
         * @param address The IP socket address to connect to.
         * @param timeout_ms Timeout in milliseconds for the connection.
         */
        TcpSocket(IpSocketAddress address, uint32_t timeout_ms);

        /**
         * @brief Constructs a TcpSocket with a local address and port, and accepts a client connection.
         * @param local_address The local IP socket address to bind to.
         * @param timeout_ms Timeout in milliseconds for accepting a client.
         * @param reuse_addr Whether to reuse the address if it is managed by OS (SO_REUSEADDR flag).
         */
        TcpSocket(IpSocketAddress local_address, uint32_t timeout_ms, bool reuse_addr);

        /**
        * @brief Destructor for TcpSocket.
        */
        virtual ~TcpSocket();

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
         * @Note This method blocks until the data is received or an timeout/error occurs.
         */
		bool receive(void* data, size_t size, size_t& num_recvd);

		/**
         * @brief Receives data from the socket.
         * @param data Pointer to the buffer to store received data.
         * @param size Size of the buffer.
         * @Note This method blocks until the data is received or an error/timeout occurs.
         * @throws PosixException on timeout and other errors.
         */
		void receive(void* data, size_t size);

	private:
		friend sptr TcpServerSocket::accept_connection();
		friend sptr TcpServerSocket::accept_connection(uint32_t);

		explicit TcpSocket(Handle&& sockfd); // take over existing sock fd given by TcpServerSocket::accept_connection()

};

