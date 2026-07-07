#include <gtest/gtest.h>

#include "qf/Models/GeometricBrownianMotion.hpp"
#include "qf/Random/RandomGenerator.hpp"
#include "qf/Statistics/RunningStats.hpp"

#include <cmath>

TEST(GeometricBrownianMotion, ZeroVolatilityProducesDeterministicGrowth)
{
    constexpr double initialPrice   = 100.0;
    constexpr double drift          = 0.05;
    constexpr double volatility     = 0.0;
    constexpr double timeStep       = 1.0;

    qf::GeometricBrownianMotion gbm(drift, volatility);
    qf::RandomGenerator randomGenerator(42);

    const double evolvedPrice = gbm.Evolve(initialPrice, timeStep, randomGenerator);

    const double expectedPrice = initialPrice * std::exp(drift * timeStep);

    EXPECT_NEAR(evolvedPrice, expectedPrice, 1e-12);
}

TEST(GeometricBrownianMotion, SameSeedProducesIdenticalEvolution)
{
    constexpr double initialPrice   = 100.0;
    constexpr double drift          = 0.05;
    constexpr double volatility     = 0.20;
    constexpr double timeStep       = 1.0 / 252.0;

    qf::GeometricBrownianMotion gbm(drift, volatility);

    qf::RandomGenerator randomGenerator1(42);
    qf::RandomGenerator randomGenerator2(42);

    const double price1 = gbm.Evolve(initialPrice, timeStep, randomGenerator1);

    const double price2 = gbm.Evolve(initialPrice, timeStep, randomGenerator2);

    EXPECT_DOUBLE_EQ(price1, price2);
}

TEST(GeometricBrownianMotion, LogReturnsHaveCorrectStatistics)
{
    constexpr double initialPrice   = 100.0;
    constexpr double drift          = 0.05;
    constexpr double volatility     = 0.20;
    constexpr double timeStep       = 1.0 / 252.0;

    constexpr std::size_t numberOfSamples = 100000;

    qf::GeometricBrownianMotion gbm(drift, volatility);
    qf::RandomGenerator randomGenerator(42);
    qf::RunningStats statistics;

    for (std::size_t i = 0; i < numberOfSamples; ++i)
    {
        const double evolvedPrice = gbm.Evolve(initialPrice, timeStep, randomGenerator);

        const double logReturn = std::log(evolvedPrice / initialPrice);

        statistics.Add(logReturn);
    }

    const double standardError = std::sqrt(statistics.Variance() / static_cast<double>(numberOfSamples)); 

    const double expectedMean = (drift - 0.5 * volatility * volatility) * timeStep;

    const double expectedVariance = volatility * volatility * timeStep;

    // Monte Carlo estimates converge as O(1/sqrt(N)).
    // Validate that the observed mean is within three standard errors
    // of the analytical expectation.
    EXPECT_NEAR(statistics.Mean(), expectedMean, 3.0 * standardError);

    EXPECT_NEAR(statistics.Variance(), expectedVariance, 1e-5);
}