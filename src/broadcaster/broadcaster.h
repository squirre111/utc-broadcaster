#pragma once

#include "udp.h"
#include "sock_addr.h"
#include "factories.h"

#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>


/**
   Интерфейс приложения
*/
class Runnable {
public:
    virtual void Run() = 0;
    virtual void Stop() = 0;
    virtual bool IsRunning() const = 0;
    virtual ~Runnable() {}
};


/**
   Основной класс приложения, поддерживает работу с различными фабриками из factories.h
*/
template<typename T = TimestampFactory>
class Broadcaster : public T, public Runnable {

public:
    
    Broadcaster(size_t port, size_t timeout) :
        addr_(SockAddr::Type::BROADCAST, port),
        sock_(), timeout_(timeout), wakeup_(false), runned_(false)
    {
        sock_.AllowBroadcast();
    }
    
    
    /** Запуск потоков таймера и отправки */
    void Run() override {
        std::lock_guard<std::mutex> guard(m_);
        
        if (thread_timer_.joinable() || thread_sender_.joinable()) return;
        runned_ = true;
        thread_timer_ = std::thread(&Broadcaster::ThreadTimer, this);
        thread_sender_ = std::thread(&Broadcaster::ThreadSender, this);
    }
    
    
    /** Остановка отправки */
    void Stop() override {
        if (!thread_timer_.joinable() || !thread_sender_.joinable()) return;
        
        {
            std::lock_guard<std::mutex> guard(m_);
            wakeup_ = true;
        }
        runned_ = false;
        cv_.notify_one();
        
        thread_timer_.join();
        thread_sender_.join();
    }


    bool IsRunning() const override {
        return thread_timer_.joinable() && thread_sender_.joinable();
    }
    
    
    /** 
        Основной цикл отправки уведомлений cv_ о необходимости начать передачу
    */
    void ThreadTimer() {
        while(runned_) {
            SystemClock::SleepMS(timeout_);
            std::lock_guard<std::mutex> guard(m_);
            wakeup_ = true;
            cv_.notify_one();
        }
    }
    
    
    /*
        Поток в котором отправляются датаграммы по уведомлению от cv_
        Отправка вынесена в отдельный поток, чтобы тайминг был максимально точен
    */
    void ThreadSender() {
        while(1) {
            {
                std::unique_lock<std::mutex> guard(m_);
                cv_.wait(guard, [&](){ return wakeup_; });
                wakeup_ = false;
            }
            // вынесено из-под защиты guard чтобы как можно меньше держать его закрытым,
            if (!runned_) break;
            Broadcast();
        }
    }
    
    
    /** Отправка одной датаграммы */
    void Broadcast() const {
        auto packet = T::BuildPacket();
        UDP::SendTo(sock_, addr_, packet.get());
    }
    
    ~Broadcaster() {
        Stop();
    }

private:
    SockAddr addr_;
    UDP::Socket sock_;
    size_t timeout_;
    
    std::thread thread_timer_;
    std::thread thread_sender_;
    std::mutex m_;
    std::condition_variable cv_;
    bool wakeup_;
    
    std::atomic<bool> runned_;
};
