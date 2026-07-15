#include <gtest/gtest.h>

#include "qf/Pricing/Analytical/BlackScholesEuropeanPricer.hpp"
#include "qf/Products/OptionType.hpp"

TEST(BlackScholesPricer, EuropeanCallPrice)
{
    constexpr double strike = 100.0;
    constexpr double timeToMaturity = 1.0;

    constexpr double spot = 100.0;
    constexpr double riskFreeRate = 0.05;
    constexpr double volatility = 0.20;

    constexpr double referencePrice = 10.4506;

    qf::MarketScenario scenario;

    scenario.Spot = spot;
    scenario.Volatility = volatility;
    scenario.RiskFreeRate = riskFreeRate;

    qf::BlackScholesEuropeanPricer pricer(qf::OptionType::Call, strike, timeToMaturity);

    const double price = pricer.Price(scenario).Value;

    EXPECT_NEAR(price, referencePrice, 1e-4);
}

TEST(BlackScholesPricer, EuropeanPutPrice)
{
    constexpr double strike = 100.0;
    constexpr double timeToMaturity = 1.0;

    constexpr double spot = 100.0;
    constexpr double riskFreeRate = 0.05;
    constexpr double volatility = 0.20;

    constexpr double referencePrice = 5.5735;

    qf::MarketScenario scenario;

    scenario.Spot = spot;
    scenario.Volatility = volatility;
    scenario.RiskFreeRate = riskFreeRate;

    qf::BlackScholesEuropeanPricer pricer(qf::OptionType::Put, strike, timeToMaturity);

    EXPECT_NEAR(pricer.Price(scenario).Value, referencePrice, 1e-4);
}