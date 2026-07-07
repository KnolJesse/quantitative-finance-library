#include <gtest/gtest.h>

#include "qf/Models/GeometricBrownianMotion.hpp"
#include "qf/MonteCarlo/PathGenerator.hpp"
#include "qf/Random/RandomGenerator.hpp"

TEST(PathGenerator, GeneratesCorrectNumberOfPoints)
{
    constexpr double drift = 0.05;
    constexpr double volatility = 0.20;

    constexpr double initialPrice = 100.0;
    constexpr double timeStep = 1.0 / 252.0;

    constexpr std::size_t numberOfSteps = 10;

    qf::GeometricBrownianMotion gbm(drift, volatility);
    qf::RandomGenerator randomGenerator(42);

    qf::PathGenerator generator(
        gbm,
        randomGenerator,
        initialPrice,
        timeStep,
        numberOfSteps);

    const qf::Path path = generator.Generate();

    EXPECT_EQ(path.Size(), numberOfSteps + 1);
    EXPECT_DOUBLE_EQ(path.At(0), initialPrice);
}