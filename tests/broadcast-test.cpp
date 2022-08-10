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



TEST(BroadcastTest, CycleTransmitTest)
{
    size_t port = 12345;
    Broadcaster<TimestampFactory> b(port, 100);
    Reciever r(port);
    FixedPacket<sizeof(SystemClock::ms_size)> packet;
    
    size_t count_packets = 0;
    std::atomic<bool> t1_run{true};

    std::thread t1([&] {
            while(t1_run) {
                if (r.GetPacket(&packet)) {
                    count_packets++;
                }
            }
        });
    
    b.Run();
    SystemClock::SleepMS(1000);
    b.Stop();
    
    t1_run = false;
    b.Broadcast(); // dirty hack to unblock recvfrom
    
    t1.join();
    
    EXPECT_GE(count_packets, 10);
    EXPECT_LT(count_packets, 12);
}
