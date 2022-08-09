#pragma once

#include <memory>
#include <string>

#include "packets.h"
#include "system_clock.h"

/**
   Фабрика создающая бинарные пакеты с UTC Timestamp
   Основная рабочая фабрика
*/
class TimestampFactory {
public:
    static std::unique_ptr<Packet> BuildPacket();
    static std::string PrintPacket(Packet *);
    static SystemClock::ms_size ParsePacket(Packet *);
};

/**
   Фабрика создающая пакеты со строковым представлением UTC Timestamp

   Представлена для примера возможности работы Broadcaster с разными фабриками
*/
class TimestampStringFactory {
public:
    static std::unique_ptr<Packet> BuildPacket();
    static std::string PrintPacket(Packet *);
    static SystemClock::ms_size ParsePacket(Packet *);
};

/**
   Фабрика создающая пакеты со содержимым str_,
   которое можно менять в процессе работы

   Представлена для примера возможности работы Broadcaster с разными фабриками
*/
class ConstFactory {
public:
    std::unique_ptr<Packet> BuildPacket() const;
    void SetConst(const std::string &str) {
        str_ = str;
    }
    std::string str_;

    static std::string PrintPacket(Packet *);
};
