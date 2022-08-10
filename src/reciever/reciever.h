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
    
    /** Функция для вызова из main(), основной обработчик */
    int Run() {
        
        FixedPacket<sizeof(SystemClock::ms_size)> packet;
        
        while(1) {
            if (GetPacket(&packet)) {
                auto currentMs = SystemClock::GetMS();
                auto packetMs = TimestampFactory::ParsePacket(&packet);
                std::cout << PrintDiff(packetMs, currentMs) << std::endl;
            }

        }
        return EXIT_SUCCESS;
    }

    static std::string PrintDiff(SystemClock::ms_size currentMs, SystemClock::ms_size packetMs) {
        auto diff = currentMs - packetMs;
        return std::to_string(currentMs) + " " + std::to_string(packetMs) + 
            " " + std::to_string(diff);
    }

    //     return std::to_string(current) + " " 
    //                   << packetMs << " " 
    //                   << diff << std::endl;
    // }

    bool GetPacket(Packet *packet) {
        size_t len = UDP::RecvFrom(sock_, packet);
        return len == packet->Size();
    }
    
    SockAddr addr_;
    UDP::Socket sock_;
};
