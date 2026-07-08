#include <gtest/gtest.h>

#include "qf/Models/GeometricBrownianMotion.hpp"
#include "qf/Pricing/MonteCarloPricer.hpp"
#include "qf/MonteCarlo/PathGenerator.hpp"
#include "qf/Payoffs/EuropeanCallPayoff.hpp"
#include "qf/Random/RandomGenerator.hpp"

#include <cmath>

TEST(MonteCarloPricer, ZeroVolatilityProducesDeterministicPrice)
{
    constexpr double initialPrice = 100.0;
    constexpr double strike = 100.0;
    constexpr double drift = 0.05;
    constexpr double volatility = 0.0;

    constexpr double riskFreeRate = 0.05;
    constexpr double maturity = 1.0;

    constexpr double timeStep = 1.0;
    constexpr std::size_t numberOfSteps = 1;
    constexpr std::size_t simulations = 1000;

    qf::RandomGenerator randomGenerator(42);

    qf::GeometricBrownianMotion model(drift, volatility);

    qf::PathGenerator<qf::GeometricBrownianMotion> generator(
        model,
        randomGenerator,
        initialPrice,
        timeStep,
        numberOfSteps);

    qf::EuropeanCallPayoff payoff(strike);

    qf::MonteCarloPricer pricer(
        generator,
        payoff,
        riskFreeRate,
        maturity,
        simulations);

    const qf::PricingResult price = pricer.Price();

    const double terminalPrice = initialPrice * std::exp(drift * maturity);

    const double expectedPrice = std::exp(-riskFreeRate * maturity) * std::max(terminalPrice - strike, 0.0);

    EXPECT_NEAR(price.Value, expectedPrice, 1e-12);
}

TEST(MonteCarloPricer, SameSeedProducesSamePrice)
{
    constexpr double initialPrice = 100.0;
    constexpr double strike = 100.0;

    constexpr double drift = 0.05;
    constexpr double volatility = 0.20;

    constexpr double riskFreeRate = 0.05;
    constexpr double maturity = 1.0;

    constexpr double timeStep = 0.01;
    constexpr std::size_t numberOfSteps = 100;
    constexpr std::size_t simulations = 10000;

    qf::RandomGenerator randomGenerator1(42);
    qf::RandomGenerator randomGenerator2(42);

    qf::GeometricBrownianMotion model1(drift, volatility);
    qf::GeometricBrownianMotion model2( drift, volatility);

    qf::EuropeanCallPayoff payoff1(strike);
    qf::EuropeanCallPayoff payoff2(strike);

    qf::PathGenerator<qf::GeometricBrownianMotion> generator1(
        model1,
        randomGenerator1,
        initialPrice,
        timeStep,
        numberOfSteps);

    qf::PathGenerator<qf::GeometricBrownianMotion> generator2(
        model2,
        randomGenerator2,
        initialPrice,
        timeStep,
        numberOfSteps);

    qf::MonteCarloPricer<qf::GeometricBrownianMotion> pricer1(
        generator1,
        payoff1,
        riskFreeRate,
        maturity,
        simulations);

    qf::MonteCarloPricer<qf::GeometricBrownianMotion> pricer2(
        generator2,
        payoff2,
        riskFreeRate,
        maturity,
        simulations);

    const qf::PricingResult price1 = pricer1.Price();
    const qf::PricingResult price2 = pricer2.Price();

    EXPECT_NEAR(price1.Value, price2.Value, 1e-12);
    EXPECT_NEAR(price1.StandardError, price2.StandardError, 1e-12);
}