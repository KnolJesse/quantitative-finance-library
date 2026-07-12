#include <gtest/gtest.h>

#include "qf/PathGeneration/Path.hpp"
#include "qf/Payoffs/EuropeanPutPayoff.hpp"

TEST(EuropeanPutPayoff, InTheMoney)
{
    qf::EuropeanPutPayoff payoff(100.0);

    qf::Path path({ 0.0 }, { 75.0 });

    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 25.0);
}

TEST(EuropeanPutPayoff, AtTheMoney)
{
    qf::EuropeanPutPayoff payoff(100.0);

    qf::Path path({ 0.0 }, { 100.0 });

    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 0.0);
}

TEST(EuropeanPutPayoff, OutOfTheMoney)
{
    qf::EuropeanPutPayoff payoff(100.0);

    qf::Path path({ 0.0 }, { 125.0 });

    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 0.0);
}