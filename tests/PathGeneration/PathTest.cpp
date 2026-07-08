#include <gtest/gtest.h>

#include "qf/PathGeneration/Path.hpp"


TEST(Path, StoresAndReturnsValues)
{
    qf::Path path({ 1.0, 2.0, 3.0 });

    EXPECT_EQ(path.Size(), 3u);

    EXPECT_DOUBLE_EQ(path.At(0), 1.0);
    EXPECT_DOUBLE_EQ(path.At(1), 2.0);
    EXPECT_DOUBLE_EQ(path.At(2), 3.0);
}