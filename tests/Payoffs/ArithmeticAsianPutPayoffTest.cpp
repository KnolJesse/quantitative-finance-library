#include <gtest/gtest.h>

#include "qf/PathGeneration/Path.hpp"
#include "qf/Payoffs/ArithmeticAsianPutPayoff.hpp"

TEST(ArithmeticAsianPutPayoff, OutOfTheMoney)
{
    qf::ArithmeticAsianPutPayoff payoff(95.0);

    qf::Path path({90.0, 100.0, 110.0});

    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 0.0);
}

TEST(ArithmeticAsianPutPayoff, AtTheMoney)
{
    qf::ArithmeticAsianPutPayoff payoff(100.0);

    qf::Path path({90.0, 100.0, 110.0});

    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 0.0);
}

TEST(ArithmeticAsianPutPayoff, InTheMoney)
{
    qf::ArithmeticAsianPutPayoff payoff(105.0);

    qf::Path path({90.0, 100.0, 110.0});

    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 5.0);
}