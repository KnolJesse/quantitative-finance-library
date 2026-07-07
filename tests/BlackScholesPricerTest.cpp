#include <gtest/gtest.h>

#include "qf/Pricing/BlackScholesPricer.hpp"

TEST(BlackScholesPricer, KnownEuropeanCallPrice)
{
    constexpr double spot = 100.0;
    constexpr double strike = 100.0;
    constexpr double riskFreeRate = 0.05;
    constexpr double volatility = 0.20;
    constexpr double maturity = 1.0;

    constexpr double referencePrice = 10.4506;

    qf::BlackScholesPricer pricer(
        spot,
        strike,
        riskFreeRate,
        volatility,
        maturity);

    const double price = pricer.Price();

    EXPECT_NEAR(price, referencePrice, 1e-4);
}