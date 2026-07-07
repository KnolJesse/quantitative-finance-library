#include <gtest/gtest.h>

#include "qf/Models/GeometricBrownianMotion.hpp"
#include "qf/MonteCarlo/MonteCarloPricer.hpp"
#include "qf/MonteCarlo/PathGenerator.hpp"
#include "qf/Pricing/BlackScholesPricer.hpp"
#include "qf/Pricing/PricingResult.hpp"
#include "qf/Payoffs/EuropeanCallPayoff.hpp"
#include "qf/Random/RandomGenerator.hpp"

TEST(PricingComparison, MonteCarloMatchesBlackScholes)
{
    constexpr double spot = 100.0;
    constexpr double strike = 100.0;
    constexpr double riskFreeRate = 0.05;
    constexpr double volatility = 0.20;
    constexpr double maturity = 1.0;

    constexpr double timeStep = 0.01;
    constexpr std::size_t numberOfSteps = 100;
    constexpr std::size_t simulations = 1'000'000;

    qf::BlackScholesPricer analyticPricer(
        spot,
        strike,
        riskFreeRate,
        volatility,
        maturity);

    const double analyticalPrice = analyticPricer.Price();

    qf::RandomGenerator randomGenerator(42);

    qf::GeometricBrownianMotion model(riskFreeRate, volatility);

    qf::PathGenerator<qf::GeometricBrownianMotion> pathGenerator(
        model,
        randomGenerator,
        spot,
        timeStep,
        numberOfSteps);

    qf::EuropeanCallPayoff payoff(strike);

    qf::MonteCarloPricer<qf::GeometricBrownianMotion> monteCarloPricer(
        pathGenerator,
        payoff,
        riskFreeRate,
        maturity,
        simulations);

    const qf::PricingResult monteCarloResult = monteCarloPricer.Price();

    const double error = std::abs(monteCarloResult.Value - analyticalPrice); 

    EXPECT_LT(error, 3.0 * monteCarloResult.StandardError); 
}