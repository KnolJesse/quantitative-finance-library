#include <gtest/gtest.h>
#include "qf/Random/RandomGenerator.hpp"
#include "qf/statistics/RunningStats.hpp"

TEST(RandomGenerator, ReproducibleWithSameSeed)
{
    qf::RandomGenerator randomGenerator1(42); 
    qf::RandomGenerator randomGenerator2(42); 

    EXPECT_EQ(randomGenerator1.Uniform(), randomGenerator2.Uniform());
    EXPECT_EQ(randomGenerator1.Normal(), randomGenerator2.Normal());
}

TEST(RandomGenerator, UniformValuesAreInRange)
{
    for (std::size_t i = 0; i < 1000; ++i)
    {
        qf::RandomGenerator randomGenerator(42);

        const double value = randomGenerator.Uniform();

        EXPECT_GE(value, 0.0);
        EXPECT_LT(value, 1.0);
    }
}

TEST(RandomGenerator, NormalDistributionHasExpectedStatistics)
{
    qf::RandomGenerator randomGenerator(42);
    qf::RunningStats stats;

    constexpr std::size_t sampleCount = 100000;

    for (std::size_t i = 0; i < sampleCount; ++i)
    {
        stats.Add(randomGenerator.Normal());
    }

    EXPECT_NEAR(stats.Mean(), 0.0, 0.01);
    EXPECT_NEAR(stats.Variance(), 1.0, 0.02);
}

TEST(RandomGenerator, CloneProducesIdenticalSequence)
{
    qf::RandomGenerator original(42);

    qf::RandomGenerator clone = original.Clone();

    for (std::size_t i = 0; i < 100; ++i)
    {
        EXPECT_DOUBLE_EQ(original.Normal(), clone.Normal());
    }
}