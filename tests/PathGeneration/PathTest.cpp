#include <gtest/gtest.h>

#include "qf/PathGeneration/Path.hpp"


TEST(Path, StoresAndReturnsValues)
{
    qf::Path path({ 1.0, 2.0, 3.0 });

    EXPECT_EQ(path.size(), 3u);

    EXPECT_DOUBLE_EQ(path.at(0), 1.0);
    EXPECT_DOUBLE_EQ(path.at(1), 2.0);
    EXPECT_DOUBLE_EQ(path.at(2), 3.0);
}