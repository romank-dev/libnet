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


#include <libnet/UnixDatagramSocket.hpp>
#include <libcommon/libcommon.hpp>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

UnixDatagramSocket::UnixDatagramSocket(UnixSocketAddress local_address) :
	UnixSocket(Protocol::DGRAM, local_address)
{}

UnixDatagramSocket::UnixDatagramSocket() :
    UnixSocket(Protocol::DGRAM)
{}

void UnixDatagramSocket::send(const void* data, size_t size, UnixSocketAddress target)
{
	CHECK_THROW(size <= 65507, "send(): UDP packet size too large");
    CHECK_THROW_POSIX(-1 != sendto(_sockfd, data, size, 0, (sockaddr*)(&target), target.true_size()),"UnixDatagramSocket::send(): send failed");
}

bool UnixDatagramSocket::receive(void* data, size_t size, uint16_t& packet_size, UnixSocketAddress& from)
{
    CHECK_THROW(owns_address(), "cannot receive on an unbound socket")
	socklen_t addrlen = sizeof(from);
	sockaddr_un& from_addr = from;
	int res = recvfrom(_sockfd, data, size, 0, (sockaddr*)(&from_addr), &addrlen);
	CHECK_THROW_POSIX(res >= 0 || errno == EAGAIN || errno == EWOULDBLOCK, "recvfrom() failed");
	packet_size = (uint16_t)res;
	return res >= 0;
}









