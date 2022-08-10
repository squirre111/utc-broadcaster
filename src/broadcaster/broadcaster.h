#pragma once

#include "udp.h"
#include "sock_addr.h"
#include "factories.h"

#include <thread>
#include <mutex>
#include <condition_variable>
        


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
        sock_(), timeout_(timeout), wakeup_(false)
    {
        sock_.AllowBroadcast();
        t_ = std::thread(&Broadcaster::Thread, this);
    }

    /** 
        Функция для вызова из main(), основной цикл отправки 
        уведомлений cv_ о необходимости начать передачу
    */
    int Run() override {
        while(1) {
            SystemClock::SleepMS(timeout_);
            std::lock_guard<std::mutex> guard(m_);
            wakeup_ = true;
            cv_.notify_one();
        }
        return EXIT_SUCCESS;
    }
    
    /** 
        Поток в котором отправляются датаграммы по уведомлению от cv_
        Отправка вынесена в отдельный поток, чтобы тайминг был максимально точен
    */
    void Thread() {
        for(;;) {
            std::unique_lock<std::mutex> guard(m_);
            cv_.wait(guard, [&](){ return wakeup_; });
            Broadcast();
            wakeup_ = false;
        }
    }

    /** Отправка одной датаграммы */
    void Broadcast() const {
        auto packet = T::BuildPacket();
        SendPacket(packet.get());
        SystemClock::SleepMS(1800);
    }

    /** Отправка произвольного пакета */
    void SendPacket(Packet *packet) const {
        UDP::SendTo(sock_, addr_, *packet);
    }
    
    ~Broadcaster() {
        t_.join();
    }

private:
    SockAddr addr_;
    UDP::Socket sock_;
    size_t timeout_;
    
    std::thread t_;
    std::mutex m_;
    std::condition_variable cv_;
    bool wakeup_;
};
