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
#include <libcommon/libcommon.hpp>

class Socket : NonCopyable
{
    public:
        enum class Type
        {
            TCP,
            UDP
        };


    public:
		virtual ~Socket();

		void set_send_timeout(int ms);
		void set_receive_timeout(int ms);

		void bind_to_address(const sockaddr_in& addr);
		void bind_to_port(uint16_t port);
		void bind_to_device(const std::string& iface_name);

		void set_sock_opt(int level, int optname, void* optval, socklen_t optlen);
		int get_sock_opt(int level, int optname, void* optval, socklen_t* optlen);

		static struct sockaddr_in addr_from_string(const std::string& ip, uint16_t port);

    protected:
		explicit Socket(Type sock_type);
        Socket(Type sock_type, uint16_t local_port, bool reuse_addr=false); // socket() + bind()
        explicit Socket(int sockfd); // take over existing socket

	protected:
		int 				_sockfd;
};

typedef std::shared_ptr<Socket> SocketPtr;
