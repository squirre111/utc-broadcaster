#include "gtest/gtest.h"

#include "sock_addr.h"
#include "udp.h"
#include "packets.h"
#include "system_clock.h"
#include "factories.h"


TEST(UtilsTest, SockAddrTest)
{
    {
        SockAddr addr(SockAddr::Type::ANY, 12345);
        EXPECT_EQ (addr.Port(),  12345);
        EXPECT_EQ (addr.Str(),  "0.0.0.0");
    }
    {
        SockAddr addr(SockAddr::Type::BROADCAST, 123);
        EXPECT_EQ (addr.Port(),  123);
        EXPECT_EQ (addr.Str(),  "255.255.255.255");
    }
    {
        SockAddr addr("192.168.1.1", 8000);
        EXPECT_EQ (addr.Port(),  8000);
        EXPECT_EQ (addr.Str(),  "192.168.1.1");
    }

}


TEST(UtilsTest, UdpSockTest)
{
    {
        UDP::Socket sock1;
        EXPECT_GT (sock1.FD(),  STDERR_FILENO);
        EXPECT_EQ (sock1.HasBroadcast(),  false);
        sock1.AllowBroadcast();
        EXPECT_EQ (sock1.HasBroadcast(),  true);
        auto sock1_fd_saved = sock1.FD();
        
        UDP::Socket sock2;
        EXPECT_GT (sock2.FD(),  sock1.FD());
        
        sock1.Close();
        sock2.Close();

        UDP::Socket sock3;
        EXPECT_EQ (sock3.FD(),  sock1_fd_saved);
    }
}


TEST(UtilsTest, BindTest)
{
    {
        UDP::Socket socket;
        SockAddr addr(SockAddr::Type::ANY, 12345);
        EXPECT_EQ (socket.Bind(addr), true);
        EXPECT_EQ (socket.Bind(addr), false); // again
    }
}


TEST(UtilsTest, SystemClockTest)
{
    {
		auto ms1 = SystemClock::GetMS();
		auto ms2 = SystemClock::GetMS();
        EXPECT_LE (ms1,  ms2); // less or equal
    }
    {
		auto ms1 = SystemClock::GetMS();
		SystemClock::SleepMS(1);
		auto ms2 = SystemClock::GetMS();
        EXPECT_LT (ms1,  ms2); // strict less
    }
    {
		auto ms = SystemClock::GetMS();
		std::string ms_str = std::to_string(ms);
		EXPECT_EQ (ms_str.size(), MS_STR_LEN);
    }
}


TEST(UtilsTest, PacketsTest)
{
    {
		FixedPacket<4> packet;
        EXPECT_EQ (packet.Size(),  4);
    }
    {
        std::string str("foobar");
        DynamicPacket packet;
        packet.SetData(str.c_str(), str.size());
        EXPECT_EQ (packet.Size(),  str.size());

        // resize
        std::string str2("foobarbaz");
        packet.SetData(str2.c_str(), str2.size());
        EXPECT_EQ (packet.Size(),  str2.size());
    }
}


TEST(UtilsTest, FactoriesTest)
{
    {
        auto packet1 = TimestampFactory::BuildPacket();
        EXPECT_EQ (packet1->Size(),  sizeof(SystemClock::ms_size));
        auto packet1_val = TimestampFactory::ParsePacket(packet1.get());
        auto packet1_str = TimestampFactory::PrintPacket(packet1.get());
        EXPECT_EQ (packet1_str.size(), MS_STR_LEN);
        
        auto packet2 = TimestampStringFactory::BuildPacket();
        EXPECT_EQ (packet2->Size(),  MS_STR_LEN);
        auto packet2_val = TimestampStringFactory::ParsePacket(packet2.get());
        auto packet2_str = TimestampStringFactory::PrintPacket(packet2.get());
        EXPECT_EQ (packet2_str.size(), MS_STR_LEN);
        
        EXPECT_LE (packet1_val,  packet2_val); // less or equal

        // try to use wrong parser
        auto packet1_wrong_val = TimestampStringFactory::ParsePacket(packet1.get());
        EXPECT_EQ (packet1_wrong_val, 0);

        // try to use wrong parser
        auto packet2_wrong_val = TimestampFactory::ParsePacket(packet2.get());
        EXPECT_EQ (packet2_wrong_val, 0);
    }
    {
        std::string str("foobaz");
        ConstFactory factory;
        factory.SetConst(str);
        
        auto packet = factory.BuildPacket();
        EXPECT_EQ (packet->Size(), str.size());
        EXPECT_EQ (str,  ConstFactory::PrintPacket(packet.get()));
    }
}


TEST(UtilsTest, UDPSendTest)
{
    {
        UDP::Socket socket;
        socket.AllowBroadcast();
        
        std::string str("foobar");
        DynamicPacket packet;
        packet.SetData(str.c_str(), str.size());
        
        SockAddr addr(SockAddr::Type::ANY, 12345);
        auto len = UDP::SendTo(socket, addr, packet);
        EXPECT_EQ (packet.Size(), str.size());
        EXPECT_EQ (len, packet.Size());
    }
}
