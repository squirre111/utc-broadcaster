#include "gtest/gtest.h"

#include "broadcaster.h"
#include "reciever.h"

#include <thread>


TEST(BroadcastTest, TransmitTest)
{
    size_t port = 12345;
    Broadcaster<TimestampFactory> b(port, 100);

    auto current_ms = SystemClock::GetMS();
    SystemClock::SleepMS(100);
    
    bool parse_result = false;
    FixedPacket<sizeof(SystemClock::ms_size)> packet;
    std::thread t1([&] {
            Reciever r(port);
            parse_result = r.GetPacket(&packet);
        });
    
    std::thread t2([&] {
            b.Broadcast();
        });
    
    t1.join();
    t2.join();
    EXPECT_EQ(parse_result, true);
    
    SystemClock::ms_size packet_ms = TimestampFactory::ParsePacket(&packet);
    
    auto diff = packet_ms - current_ms;
    EXPECT_GE(diff, 100);
    EXPECT_LT(diff, 110);
}
