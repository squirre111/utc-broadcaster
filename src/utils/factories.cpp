#include "factories.h"

#include "system_clock.h"

#include <cassert>
#include <string>
#include <iostream>


static std::string Packet2String(Packet *packet)
{
    return std::string(reinterpret_cast<const char *>(packet->Data()), packet->Size());
}


std::unique_ptr<Packet> TimestampFactory::BuildPacket()
{
    auto ms = SystemClock::GetMS();
    auto packet = new FixedPacket<sizeof(ms)>();
    packet->SetData(&ms, sizeof(ms));
    return std::unique_ptr<Packet>(packet);
}


std::string TimestampFactory::PrintPacket(Packet *packet)
{
    return std::to_string(ParsePacket(packet));
}


SystemClock::ms_size TimestampFactory::ParsePacket(Packet *packet)
{
    SystemClock::ms_size dst;
    if (packet->Size() != sizeof(dst))
        return 0;
    memcpy(&dst, packet->Data(), packet->Size());
    return dst;
}


std::unique_ptr<Packet> TimestampStringFactory::BuildPacket()
{
    auto ms = SystemClock::GetMS();
    std::string ms_str = std::to_string(ms);
    assert(ms_str.size() == MS_STR_LEN);
    
    auto const packetSize = MS_STR_LEN;
    
    auto packet = new FixedPacket<packetSize>();
    packet->SetData(ms_str.c_str(), packetSize);
    
    return std::unique_ptr<Packet>(packet);
}


std::string TimestampStringFactory::PrintPacket(Packet *packet)
{
    return Packet2String(packet);
}


SystemClock::ms_size TimestampStringFactory::ParsePacket(Packet *packet)
{
    SystemClock::ms_size res = 0;
    auto str = PrintPacket(packet);
    
    if (str.size() != MS_STR_LEN)
        return res;
    
    try {
        res = std::stoll(str);
    }
    catch(std::invalid_argument const& ex) {
        std::cerr << "std::invalid_argument::what(): " << ex.what() << std::endl;
    }
    catch(std::out_of_range const& ex) {
        std::cerr << "std::out_of_range::what(): " << ex.what() << std::endl;
    }
    return res;
}


std::unique_ptr<Packet> ConstFactory::BuildPacket() const
{
    auto packet = new DynamicPacket();
    packet->SetData(str_.c_str(), str_.size());
    
    return std::unique_ptr<Packet>(packet);
}


std::string ConstFactory::PrintPacket(Packet *packet)
{
    return Packet2String(packet);
}
