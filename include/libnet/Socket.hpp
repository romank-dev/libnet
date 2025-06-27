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

class Socket : NonCopyable
{
    public:

        using sptr = std::shared_ptr<Socket>;
        using uptr = std::unique_ptr<Socket>;

        enum class SockType
        {
            IPv4,
            UNIX
        };

    public:
        virtual ~Socket();

        SockType type() const;

        void set_send_timeout(uint32_t ms);
        void set_receive_timeout(uint32_t ms);

        void set_sock_opt(int level, int optname, void* optval, socklen_t optlen);
        void get_sock_opt(int level, int optname, void* optval, socklen_t* optlen);


    protected:
        explicit Socket(SockType type, Handle&& sockfd);

    protected:
        Handle              _sockfd;
        const SockType      _type;
};




