#include <gtest/gtest.h>

#include "qf/Market/MarketScenario.hpp"
#include "qf/Models/GeometricBrownianMotion.hpp"
#include "qf/Pricing/MonteCarlo/GBMMonteCarloPricer.hpp"
#include "qf/PathGeneration/PathGenerator.hpp"
#include "qf/Payoffs/EuropeanCallPayoff.hpp"
#include "qf/Payoffs/Payoff.hpp"
#include "qf/Random/RandomGenerator.hpp"
#include "qf/Simulation/MonteCarloSettings.hpp"


#include <cmath>

TEST(MonteCarloPricer, ZeroVolatilityProducesDeterministicPrice)
{
    constexpr double strike = 100.0;
    constexpr double timeToMaturity = 1.0;

    qf::EuropeanCallPayoff payoff(strike, timeToMaturity);

    constexpr double spot           = 100.0;
    constexpr double volatility     = 0.0;
    constexpr double riskFreeRate   = 0.05;

    qf::MarketScenario scenario;

    scenario.Spot           = spot;
    scenario.Volatility     = volatility;
    scenario.RiskFreeRate   = riskFreeRate;

    qf::MonteCarloSettings settings;
    
    settings.NumberOfSimulations = 1'000;
    settings.RequestedTimeStep   = 0.01;

    qf::RandomGenerator randomGenerator(42);

    qf::GBMMonteCarloPricer GBMpricer(
        payoff,
        randomGenerator, 
        settings); 

    const qf::PricingResult price = GBMpricer.Price(scenario);

    const double terminalPrice = spot * std::exp(riskFreeRate * timeToMaturity);

    const double expectedPrice = std::exp(-riskFreeRate * timeToMaturity) * std::max(terminalPrice - strike, 0.0);

    EXPECT_NEAR(price.Value, expectedPrice, 1e-12);
}

TEST(MonteCarloPricer, SameSeedProducesSamePrice)
{
    constexpr double strike = 100.0;
    constexpr double timeToMaturity = 1.0;

    qf::EuropeanCallPayoff payoff(strike, timeToMaturity);

    constexpr double spot           = 100.0;
    constexpr double volatility     = 0.20;
    constexpr double riskFreeRate   = 0.05;

    qf::MarketScenario scenario;

    scenario.Spot           = spot;
    scenario.Volatility     = volatility;
    scenario.RiskFreeRate   = riskFreeRate;
    
    qf::MonteCarloSettings settings; 

    settings.NumberOfSimulations = 10'000;
    settings.RequestedTimeStep   = 0.01;

    qf::RandomGenerator randomGenerator(42);

    qf::EuropeanCallPayoff payoff2(strike, timeToMaturity);

    qf::GBMMonteCarloPricer pricer1(
        payoff,
        randomGenerator,
        settings); 

    qf::GBMMonteCarloPricer pricer2(
        payoff,
        randomGenerator,
        settings);

    const qf::PricingResult price1 = pricer1.Price(scenario);
    const qf::PricingResult price2 = pricer2.Price(scenario);

    EXPECT_NEAR(price1.Value, price2.Value, 1e-12);
    EXPECT_NEAR(price1.StandardError, price2.StandardError, 1e-12);
}