#pragma once

#include "udp.h"
#include "sock_addr.h"
#include "factories.h"


class Reciever {
    
public:
    
    Reciever(size_t port) :
        addr_(SockAddr::Type::ANY, port),
        sock_()
    {
        sock_.Bind(addr_);
    }
    
    int Run() {
        
        FixedPacket<sizeof(SystemClock::ms_size)> packet;
        
        while(1) {
            size_t len = UDP::RecvFrom(sock_, packet);
            if (len == packet.Size()) {
                auto currentMs = SystemClock::GetMS();
                auto packetMs = TimestampFactory::ParsePacket(&packet);
                std::cout << currentMs << " " << packetMs << " " << currentMs - packetMs << std::endl;
            }
        }
        
        return EXIT_SUCCESS;
    }
    
    SockAddr addr_;
    UDP::Socket sock_;
};
