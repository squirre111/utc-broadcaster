#pragma once

#include "udp.h"
#include "sock_addr.h"
#include "factories.h"

/**
   Интерфейс приложения
*/
class Runnable {
public:
    virtual int Run() = 0;
};


/**
   Основной класс приложения, поддерживает работу с различными фабриками из factories.h
*/
template<typename T = TimestampFactory>
class Broadcaster : public T, public Runnable {

public:
    
    Broadcaster(size_t port, size_t timeout) :
        addr_(SockAddr::Type::BROADCAST, port),
        sock_(), timeout_(timeout)
    {
        sock_.AllowBroadcast();
    }

    /** Функция для вызова из main(), основной цикл обработки */
    int Run() override {
        while(1) {
            SystemClock::SleepMS(timeout_);
            Broadcast();
        }
        return EXIT_SUCCESS;
    }

    /** Отправить одну датаграмму */
    void Broadcast() {
        auto packet = T::BuildPacket();
        SendPacket(packet.get());
    }

    /** Отправить произвольный пакет */
    void SendPacket(Packet *packet) {
        UDP::SendTo(sock_, addr_, *packet);
    }

private:
    SockAddr addr_;
    UDP::Socket sock_;
    size_t timeout_;
};
