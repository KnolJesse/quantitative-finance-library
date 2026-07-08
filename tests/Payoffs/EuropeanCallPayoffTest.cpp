#include <gtest/gtest.h>

#include "qf/PathGeneration/Path.hpp"
#include "qf/Payoffs/EuropeanCallPayoff.hpp"

TEST(EuropeanCallPayoff, OutOfTheMoney)
{
    qf::EuropeanCallPayoff payoff(100.0);

    qf::Path path({90.0});

    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 0.0);
}

TEST(EuropeanCallPayoff, AtTheMoney)
{
    qf::EuropeanCallPayoff payoff(100.0);

    qf::Path path({100.0});

    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 0.0);
}

TEST(EuropeanCallPayoff, InTheMoney)
{
    qf::EuropeanCallPayoff payoff(100.0);

    qf::Path path({125.0});

    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 25.0);
}