#include <gtest/gtest.h>

#include "qf/Pricing/BlackScholesPricer.hpp"
#include "qf/Products/OptionType.hpp"

TEST(BlackScholesPricer, EuropeanCallPrice)
{
    constexpr double spot = 100.0;
    constexpr double strike = 100.0;
    constexpr double riskFreeRate = 0.05;
    constexpr double volatility = 0.20;
    constexpr double maturity = 1.0;

    constexpr double referencePrice = 10.4506;

    qf::BlackScholesPricer pricer(
        qf::OptionType::Call, 
        spot,
        strike,
        riskFreeRate,
        volatility,
        maturity);

    const double price = pricer.Price();

    EXPECT_NEAR(price, referencePrice, 1e-4);
}

TEST(BlackScholesPricer, EuropeanPutPrice)
{
    constexpr double spot = 100.0;
    constexpr double strike = 100.0;
    constexpr double riskFreeRate = 0.05;
    constexpr double volatility = 0.20;
    constexpr double maturity = 1.0;

    constexpr double referencePrice = 5.5735;

    qf::BlackScholesPricer pricer(
        qf::OptionType::Put,
        spot,
        strike,
        riskFreeRate,
        volatility,
        maturity);

    EXPECT_NEAR(pricer.Price(), referencePrice, 1e-4);
}