#pragma once

#include "udp.h"
#include "sock_addr.h"
#include "factories.h"


class Runnable {
public:
    virtual int Run() = 0;
};


template<typename T = TimestampFactory>
class Broadcaster : public T, public Runnable {

public:
    
    Broadcaster(size_t port, size_t timeout) :
        addr_(SockAddr::Type::BROADCAST, port),
        sock_(), timeout_(timeout)
    {
        sock_.AllowBroadcast();
    }
    
    int Run() override {
        while(1) {
            SystemClock::SleepMS(timeout_);
            Broadcast();
        }
        return EXIT_SUCCESS;
    }

    void Broadcast() {
        auto packet = T::BuildPacket();
        SendPacket(packet.get());
    }

    void SendPacket(Packet *packet) {
        UDP::SendTo(sock_, addr_, *packet);
    }

private:
    SockAddr addr_;
    UDP::Socket sock_;
    size_t timeout_;
};
