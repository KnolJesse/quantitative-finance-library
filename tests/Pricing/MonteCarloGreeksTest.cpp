#include <gtest/gtest.h>

#include "qf/Models/GeometricBrownianMotion.hpp"
#include "qf/Payoffs/EuropeanCallPayoff.hpp"
#include "qf/Pricing/BlackScholesPricer.hpp"
#include "qf/Pricing/MonteCarloPricer.hpp"
#include "qf/Products/OptionType.hpp"
#include "qf/Random/RandomGenerator.hpp"

#include <cstddef>
#include <iostream> 


TEST(MonteCarloGreeks, DeltaMatchesBlackScholes)
{
    constexpr double spot = 100.0;
    constexpr double strike = 100.0;
    constexpr double riskFreeRate = 0.05;
    constexpr double volatility = 0.20;
    constexpr double maturity = 1.0;

    constexpr double bump = 0.10;

    constexpr std::size_t numberOfSteps = 252;
    constexpr std::size_t simulations = 200000;

    qf::GeometricBrownianMotion model(riskFreeRate, volatility);

    qf::EuropeanCallPayoff payoff(strike);

    qf::RandomGenerator randomGenerator(42);

    qf::MonteCarloPricer pricer(
        model,
        randomGenerator,
        payoff,
        spot,
        riskFreeRate,
        maturity,
        1.0/252.0,
        //numberOfSteps,
        simulations);

    const double monteCarloDelta = pricer.Delta(bump);

    qf::BlackScholesPricer analyticPricer(
        qf::OptionType::Call,
        spot,
        strike,
        riskFreeRate,
        volatility,
        maturity);

    const double analyticalDelta = analyticPricer.Delta();

    EXPECT_NEAR(
        monteCarloDelta,
        analyticalDelta,
        3e-3);
}

TEST(MonteCarloGreeks, DeltaIsStableAcrossReasonableBumps)
{
    constexpr double spot = 100.0;
    constexpr double strike = 100.0;
    constexpr double riskFreeRate = 0.05;
    constexpr double volatility = 0.20;
    constexpr double maturity = 1.0;

    constexpr std::size_t numberOfSteps = 252;
    constexpr std::size_t simulations = 200000;

    qf::GeometricBrownianMotion model(riskFreeRate, volatility);

    qf::EuropeanCallPayoff payoff(strike);

    qf::BlackScholesPricer analyticPricer(
        qf::OptionType::Call,
        spot,
        strike,
        riskFreeRate,
        volatility,
        maturity);

    const double expectedDelta = analyticPricer.Delta();

    const std::vector<double> bumps =
    {
        0.01,
        0.10,
        1.00
    };

    for (const double bump : bumps)
    {
        qf::RandomGenerator randomGenerator(42);

        qf::MonteCarloPricer pricer(
            model,
            randomGenerator,
            payoff,
            spot,
            riskFreeRate,
            maturity,
            1.0 / 252.0,
            //numberOfSteps,
            simulations);

        const double delta = pricer.Delta(bump);

        EXPECT_NEAR(delta, expectedDelta, 5e-3);
    }
}