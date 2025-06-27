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

class IpSocket : public Socket
{
    public:

        using sptr = std::shared_ptr<IpSocket>;
        using uptr = std::unique_ptr<IpSocket>;

        enum class Protocol
        {
            TCP,
            UDP
        };

    public:

		void bind_to_address(const sockaddr_in& addr);
		void bind_to_port(uint16_t port);
		void bind_to_device(const std::string& iface_name);

		Protocol protocol() const;

		static struct sockaddr_in addr_from_string(const std::string& ip, uint16_t port);

    protected:
        explicit IpSocket(Protocol sock_type);
        IpSocket(Protocol sock_type, Handle&& sock_fd);
        IpSocket(Protocol sock_type, IpSocketAddress local_address, bool reuse_addr); // socket() + bind()


    protected:
        const Protocol        _protocol;

};
