#include <gtest/gtest.h>

#include "qf/Models/GeometricBrownianMotion.hpp"
#include "qf/Payoffs/EuropeanCallPayoff.hpp"
#include "qf/Pricing/Analytical/BlackScholesEuropeanPricer.hpp"
#include "qf/Pricing/MonteCarlo/GBMMonteCarloPricer.hpp"
#include "qf/Pricing/PricingResult.hpp"
#include "qf/Products/OptionType.hpp"
#include "qf/Random/RandomGenerator.hpp"
#include "qf/Simulation/MonteCarloSettings.hpp"

TEST(PricingComparison, MonteCarloMatchesBlackScholes)
{
    constexpr double strike = 100.0;
    constexpr double timeToMaturity = 1.0;

    qf::EuropeanCallPayoff payoff(strike, timeToMaturity);

    constexpr double spot = 100.0;
    constexpr double volatility = 0.20;
    constexpr double riskFreeRate = 0.05;

    qf::MarketScenario scenario;

    scenario.Spot = spot;
    scenario.Volatility = volatility;
    scenario.RiskFreeRate = riskFreeRate;

    qf::MonteCarloSettings settings;

    settings.NumberOfSimulations = 1'000;
    settings.RequestedTimeStep = 0.01;

    qf::RandomGenerator randomGenerator(42);

    qf::GBMMonteCarloPricer monteCarloPricer(
        payoff,
        randomGenerator,
        settings);

    qf::PricingResult monteCarloResult = monteCarloPricer.Price(scenario);

    qf::BlackScholesEuropeanPricer analyticPricer(
        qf::OptionType::Call, 
        strike,
        timeToMaturity);

    double monteCarloPrice = monteCarloResult.Value;
    double blackScholesPrice = analyticPricer.Price(scenario).Value; 

    double error = std::abs(monteCarloPrice - blackScholesPrice);

    EXPECT_LT(error, 3.0 * monteCarloResult.StandardError);
}