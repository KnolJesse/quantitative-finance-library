#include <gtest/gtest.h>
#include "qf/Random/RandomGenerator.hpp"
#include "qf/statistics/RunningStats.hpp"

qf::RandomGenerator first{ 42 };
qf::RandomGenerator second{ 42 };

TEST(RandomGenerator, ReproducibleWithSameSeed)
{
    EXPECT_EQ(first.Uniform(), second.Uniform());
    EXPECT_EQ(first.Normal(), second.Normal());
}

TEST(RandomGenerator, UniformValuesAreInRange)
{
    for (std::size_t i = 0; i < 1000; ++i)
    {
        const double value = first.Uniform();

        EXPECT_GE(value, 0.0);
        EXPECT_LT(value, 1.0);
    }
}

TEST(RandomGenerator, NormalDistributionHasExpectedStatistics)
{
    qf::RunningStats stats;

    constexpr std::size_t sampleCount = 100000;

    for (std::size_t i = 0; i < sampleCount; ++i)
    {
        stats.Add(first.Normal());
    }

    EXPECT_NEAR(stats.Mean(), 0.0, 0.01);
    EXPECT_NEAR(stats.Variance(), 1.0, 0.02);
}