#include <gtest/gtest.h>

#include "qf/Models/GeometricBrownianMotion.hpp"
#include "qf/Pricing/MonteCarloPricer.hpp"
#include "qf/Simulation/MonteCarloSettings.hpp"
#include "qf/PathGeneration/PathGenerator.hpp"
#include "qf/Payoffs/EuropeanCallPayoff.hpp"
#include "qf/Random/RandomGenerator.hpp"

#include <cmath>

TEST(MonteCarloPricer, ZeroVolatilityProducesDeterministicPrice)
{
    constexpr double spot = 100.0;
    constexpr double strike = 100.0;
    constexpr double drift = 0.05;
    constexpr double volatility = 0.0;

    constexpr double riskFreeRate = 0.05;
    constexpr double maturity = 1.0;

    qf::MonteCarloSettings settings;
    settings.NumberOfSimulations = 1'000;
    settings.RequestedTimeStep = 0.01;

    qf::RandomGenerator randomGenerator(42);

    qf::GeometricBrownianMotion model(drift, volatility);

    qf::PathGenerator<qf::GeometricBrownianMotion> generator(model, randomGenerator); 

    qf::EuropeanCallPayoff payoff(strike);

    qf::MonteCarloPricer pricer(
        model,
        randomGenerator,
        payoff,
        spot,
        riskFreeRate,
        maturity,
        settings); 

    const qf::PricingResult price = pricer.Price();

    const double terminalPrice = spot * std::exp(drift * maturity);

    const double expectedPrice = std::exp(-riskFreeRate * maturity) * std::max(terminalPrice - strike, 0.0);

    EXPECT_NEAR(price.Value, expectedPrice, 1e-12);
}

TEST(MonteCarloPricer, SameSeedProducesSamePrice)
{
    constexpr double spot = 100.0;
    constexpr double strike = 100.0;

    constexpr double drift = 0.05;
    constexpr double volatility = 0.20;

    constexpr double riskFreeRate = 0.05;
    constexpr double maturity = 1.0;

    qf::MonteCarloSettings settings; 
    settings.NumberOfSimulations = 10'000;
    settings.RequestedTimeStep = 0.01;

    qf::RandomGenerator randomGenerator1(42);
    qf::RandomGenerator randomGenerator2(42);

    qf::GeometricBrownianMotion model1(drift, volatility);
    qf::GeometricBrownianMotion model2(drift, volatility);

    qf::EuropeanCallPayoff payoff1(strike);
    qf::EuropeanCallPayoff payoff2(strike);

    qf::MonteCarloPricer pricer1(
        model1,
        randomGenerator1,
        payoff1,
        spot,
        riskFreeRate,
        maturity,
        settings); 

    qf::MonteCarloPricer pricer2(
        model2,
        randomGenerator2,
        payoff2,
        spot,
        riskFreeRate,
        maturity,
        settings);

    const qf::PricingResult price1 = pricer1.Price();
    const qf::PricingResult price2 = pricer2.Price();

    EXPECT_NEAR(price1.Value, price2.Value, 1e-12);
    EXPECT_NEAR(price1.StandardError, price2.StandardError, 1e-12);
}