#include <gtest/gtest.h>

#include "qf/PathGeneration/Path.hpp"
#include "qf/Payoffs/GeometricAsianCallPayoff.hpp"

TEST(GeometricAsianCallPayoff, OutOfTheMoney)
{
    qf::GeometricAsianCallPayoff payoff(105.0);

    qf::Path path({100.0, 100.0, 100.0});

    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 0.0);
}

TEST(GeometricAsianCallPayoff, AtTheMoney)
{
    qf::GeometricAsianCallPayoff payoff(100.0);

    qf::Path path({100.0, 100.0, 100.0});

    EXPECT_NEAR(payoff.Evaluate(path), 0.0, 1e-12);
}

TEST(GeometricAsianCallPayoff, InTheMoney)
{
    qf::GeometricAsianCallPayoff payoff(95.0);

    qf::Path path({100.0, 100.0, 100.0});

    EXPECT_NEAR(payoff.Evaluate(path), 5.0, 1e-12);
}

TEST(GeometricAsianCallPayoff, UsesGeometricNotArithmeticAverage)
{
    qf::GeometricAsianCallPayoff payoff(110.0);

    qf::Path path({50.0, 100.0, 200.0});

    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 0.0);
}