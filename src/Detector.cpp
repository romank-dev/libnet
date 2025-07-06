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

#include <libnet/Detector.hpp>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <net/if.h>
#include "logging.hpp"

using namespace std;

void Detector::broadcast(const string& id)
{
    if(_sock.get() == nullptr)
        _sock = UdpSocket::uptr(new UdpSocket());
    _sock->bind_to_port(DETECTOR_PORT);

    Buffer send_buf(id.c_str(), id.c_str() + id.length());

    Buffer data(65536);
    while(true)
    {
        uint16_t packet_size;
        IpSocketAddress from;
        _sock->receive(data.data(), data.size(), packet_size, from);

        TRACE_INF("Detector responding to client at %s...", inet_ntoa(from.address()));
        try
        {
            _sock->send(send_buf.data(), send_buf.size(), from);
        }
        catch(const std::exception& e)
        {
            TRACE_ERR("Detector failed to send packet: %s",e.what());
        }
    }
}

vector<pair<string, string>> Detector::collect_scan(uint32_t timeout_ms)
{
    vector<pair<string, string>> ans;

    if(_sock.get() == nullptr)
        return {};

    Stopwatch t;
    while(t.get_ms() < timeout_ms)
    {
        _sock->set_receive_timeout(10);
        char ret_buf[16]{};
        uint16_t packet_size;
        IpSocketAddress from;

        if(_sock->receive(ret_buf, sizeof(ret_buf), packet_size, from))
        {
            TRACE_INF("Address %s responds with ID: %s", inet_ntoa(from.address()), ret_buf);
            ans.push_back({ ret_buf, (string)from.address()});
        }
    }


    return ans;
}

void Detector::scan()
{
    if(_sock.get() == nullptr)
        _sock = unique_ptr<UdpSocket>(new UdpSocket());

    struct sockaddr_in addr {};
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    addr.sin_port = htons(DETECTOR_PORT);
    Buffer buf;
    _sock->send(buf.data(), buf.size(), addr);

    try
    {
        int broadcastEnable = 1;
       _sock->set_sock_opt(SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));

       inet_pton(AF_INET, "255.255.255.255", &addr.sin_addr);
       addr.sin_port = htons(DETECTOR_PORT);

       _sock->send(buf.data(), buf.size(), addr);
    }
    catch(...){}
}

vector<pair<string, string>> Detector::scan_and_collect(uint32_t timeout_ms)
{
    scan();

    TRACE_INF("Detector scan packet sent! waiting for response...");

    return collect_scan(timeout_ms);
}
