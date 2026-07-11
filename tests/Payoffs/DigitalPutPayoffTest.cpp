#include <gtest/gtest.h>

#include "qf/PathGeneration/Path.hpp"
#include "qf/Payoffs/DigitalPutPayoff.hpp"

TEST(DigitalPutPayoff, PaysOutWhenInTheMoney)
{
    qf::DigitalPutPayoff payoff(100.0);

    qf::Path path({90.0, 100.0, 90.0});

    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 1.0);
}

TEST(DigitalPutPayoff, DoesNotPayOutAtStrike)
{
    qf::DigitalPutPayoff payoff(100.0);

    qf::Path path({90.0, 100.0, 100.0});

    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 0.0);
}

TEST(DigitalPutPayoff, SupportsCustomPayout)
{
    qf::DigitalPutPayoff payoff(100.0, 50.0);

    qf::Path path({100.0, 90.0});

    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 50.0);
}