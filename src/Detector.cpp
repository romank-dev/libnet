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

using namespace std;

void Detector::broadcast(const string& id)
{
    if(_sock.get() == nullptr)
        _sock = make_shared<UdpSocket>();
    _sock->bind_to_port(DETECTOR_PORT);

    Buffer send_buf(id.c_str(), id.c_str() + id.length());

    Buffer data(65536);
    while(true)
    {
        uint16_t packet_size;
        struct sockaddr_in from{};
        _sock->receive(data.data(), data.size(), packet_size, from);

        //TRACE(LEVEL_INF,4054567065,1,0x6,"Detector responding to client at %s...",inet_ntoa(from.sin_addr));
        try
        {
            _sock->send(send_buf.data(), send_buf.size(), from);
        }
        catch(const std::exception& e)
        {
            //TRACE(LEVEL_ERR,3890196615,1,0x6,"Detector failed to send packet: %s",e.what());
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
        Buffer ret_buf(16, 0);
        uint16_t packet_size;
        struct sockaddr_in from{};

        if(_sock->receive(ret_buf.data(), ret_buf.size(), packet_size, from))
        {
            //TRACE(LEVEL_INF,1500758640,2,0x66,"Address %s responds with ID: %s",inet_ntoa(from.sin_addr),ret_buf.data());
            ans.push_back({ string((const char*)ret_buf.data(), min(packet_size, (uint16_t)ret_buf.size())), string(inet_ntoa(from.sin_addr))});
        }
    }


    return ans;
}

void Detector::scan()
{
    if(_sock.get() == nullptr)
        _sock = make_shared<UdpSocket>();

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

    //TRACE(LEVEL_INF,891468780,0,0,"Detector scan packet sent! waiting for response...");

    return collect_scan(timeout_ms);
}
