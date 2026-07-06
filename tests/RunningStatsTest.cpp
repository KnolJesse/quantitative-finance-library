#include <gtest/gtest.h>
#include "qf/Statistics/RunningStats.hpp"

TEST(RunningStats, MeanAndVariance)
{
    qf::RunningStats stats;

    stats.Add(1.0);
    stats.Add(2.0);
    stats.Add(3.0);
    stats.Add(4.0);

    EXPECT_EQ(stats.Count(), 4u);
    EXPECT_NEAR(stats.Mean(), 2.5, 1e-12);
    EXPECT_NEAR(stats.Variance(), 1.6666666666666667, 1e-12);
}