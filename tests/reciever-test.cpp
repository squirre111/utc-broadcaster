#include "gtest/gtest.h"

#include "reciever.h"

TEST(RecieverTest, DiffTest)
{
    {
        auto res = Reciever::PrintDiff(1660109524172, 1660109524169);
        EXPECT_EQ (res,  "1660109524172 1660109524169 3");
    }
    {
        auto res = Reciever::PrintDiff(1660109524172, 1660109524172);
        EXPECT_EQ (res,  "1660109524172 1660109524172 0");
    }
}

