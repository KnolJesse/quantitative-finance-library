#include <gtest/gtest.h>

#include "qf/Models/GeometricBrownianMotion.hpp"
#include "qf/PathGeneration/PathGenerator.hpp"
#include "qf/Random/RandomGenerator.hpp"

TEST(PathGenerator, GeneratesCorrectNumberOfPoints)
{
    constexpr double drift = 0.05;
    constexpr double volatility = 0.20;
    constexpr double spot = 100.0;
    constexpr double timeStep = 1.0 / 252.0;

    constexpr std::size_t numberOfSteps = 10;

    qf::GeometricBrownianMotion gbm(drift, volatility);
    qf::RandomGenerator randomGenerator(42);

    qf::PathGenerator generator(gbm, randomGenerator); 

    const qf::Path path = generator.Generate(spot, timeStep, numberOfSteps);

    EXPECT_EQ(path.size(), numberOfSteps + 1);
    EXPECT_DOUBLE_EQ(path.at(0), spot);
}