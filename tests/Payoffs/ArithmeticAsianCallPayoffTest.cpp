#include <gtest/gtest.h>

#include "qf/PathGeneration/Path.hpp"
#include "qf/Payoffs/ArithmeticAsianCallPayoff.hpp"

TEST(ArithmeticAsianCallPayoff, OutOfTheMoney)
{
    qf::ArithmeticAsianCallPayoff payoff(105.0);

    qf::Path path({0.0, 1.0, 2.0}, {90.0, 100.0, 110.0});

    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 0.0);
}

TEST(ArithmeticAsianCallPayoff, AtTheMoney)
{
    qf::ArithmeticAsianCallPayoff payoff(100.0);

    qf::Path path({0.0, 1.0, 2.0}, { 90.0, 100.0, 110.0 });

    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 0.0);
}

TEST(ArithmeticAsianCallPayoff, InTheMoney)
{
    qf::ArithmeticAsianCallPayoff payoff(95.0);

    qf::Path path({0.0, 1.0, 2.0}, { 90.0, 100.0, 110.0 });

    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 5.0);
}