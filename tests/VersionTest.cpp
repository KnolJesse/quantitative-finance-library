#include <gtest/gtest.h>
#include "qf/Version.hpp"

TEST(VersionTest, BasicValues)
{
    EXPECT_EQ(qf::VersionMajor, 0);
    EXPECT_EQ(qf::VersionMinor, 1);
}