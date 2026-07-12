#include <gtest/gtest.h>

#include "qf/PathGeneration/Path.hpp"
#include "qf/Payoffs/DigitalCallPayoff.hpp"

TEST(DigitalCallPayoff, PaysOutWhenInTheMoney)
{
    qf::DigitalCallPayoff payoff(100.0);

    qf::Path path({0.0, 1.0, 2.0}, { 90.0, 100.0, 110.0 });

    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 1.0);
}

TEST(DigitalCallPayoff, DoesNotPayOutAtStrike)
{
    qf::DigitalCallPayoff payoff(100.0);

    qf::Path path({ 0.0, 1.0, 2.0 }, {90.0, 100.0, 100.0});

    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 0.0);
}

TEST(DigitalCallPayoff, SupportsCustomPayout)
{
    qf::DigitalCallPayoff payoff(100.0, 50.0);

    qf::Path path({ 0.0, 1.0}, {100.0, 110.0});

    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 50.0);
}