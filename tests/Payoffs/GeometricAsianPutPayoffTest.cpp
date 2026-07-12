#include <gtest/gtest.h>

#include "qf/PathGeneration/Path.hpp"
#include "qf/Payoffs/GeometricAsianPutPayoff.hpp"

TEST(GeometricAsianPutPayoff, OutOfTheMoney)
{
    qf::GeometricAsianPutPayoff payoff(95.0);

    qf::Path path({ 0.0, 1.0, 2.0 }, {100.0, 100.0, 100.0});

    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 0.0);
}

TEST(GeometricAsianPutPayoff, AtTheMoney)
{
    qf::GeometricAsianPutPayoff payoff(100.0);

    qf::Path path({ 0.0, 1.0, 2.0 }, {100.0, 100.0, 100.0});

    EXPECT_NEAR(payoff.Evaluate(path), 0.0, 1e-12);
}

TEST(GeometricAsianPutPayoff, InTheMoney)
{
    qf::GeometricAsianPutPayoff payoff(105.0);

    qf::Path path({ 0.0, 1.0, 2.0 }, {100.0, 100.0, 100.0});

    EXPECT_NEAR(payoff.Evaluate(path), 5.0, 1e-12);
}

TEST(GeometricAsianPutPayoff, UsesGeometricNotArithmeticAverage)
{
    qf::GeometricAsianPutPayoff payoff(110.0);

    qf::Path path({ 0.0, 1.0, 2.0 }, {50.0, 100.0, 200.0});

    EXPECT_NEAR(payoff.Evaluate(path), 10.0, 1e-12);
}