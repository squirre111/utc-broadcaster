#pragma once

#include <memory>
#include <string>

#include "packets.h"
#include "system_clock.h"


class TimestampFactory {
public:
    static std::unique_ptr<Packet> BuildPacket();
    static std::string PrintPacket(Packet *);
    static SystemClock::ms_size ParsePacket(Packet *);
};


class TimestampStringFactory {
public:
    static std::unique_ptr<Packet> BuildPacket();
    static std::string PrintPacket(Packet *);
    static SystemClock::ms_size ParsePacket(Packet *);
};


class ConstFactory {
public:
    std::unique_ptr<Packet> BuildPacket() const;
    void SetConst(const std::string &str) {
        str_ = str;
    }
    std::string str_;

    static std::string PrintPacket(Packet *);
};
