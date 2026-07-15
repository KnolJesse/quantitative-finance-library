#pragma once

#include <cstddef>
#include <cmath>
#include <cassert>

#include "qf/Market/MarketScenario.hpp"
#include "qf/Models/GeometricBrownianMotion.hpp"
#include "qf/PathGeneration/PathGenerator.hpp"
#include "qf/PathGeneration/SimulationGrid.hpp"
#include "qf/Payoffs/Payoff.hpp"
#include "qf/Pricing/PricingResult.hpp"
#include "qf/Simulation/MonteCarloEngine.hpp"
#include "qf/Simulation/MonteCarloSettings.hpp"
#include "qf/Simulation/SimulationResult.hpp"
#include "qf/Random/RandomGenerator.hpp"

namespace qf
{
    /**
     * @brief Monte Carlo pricing engine.
     *
     * Prices financial derivatives by simulating paths of the underlying
     * asset and evaluating the supplied payoff.
     *
     * The pricing engine is parameterized by a stochastic model that
     * governs path evolution and a payoff that determines the option's
     * terminal value.
     *
     * Option sensitivities (Greeks) are estimated using central finite
     * differences with Common Random Numbers (CRN) to reduce estimator
     * variance.
     *
     * The returned standard error represents the statistical uncertainty
     * of the Monte Carlo price estimate.
     *
     * @tparam Model Stochastic model used to generate asset price paths.
     */
    class GBMMonteCarloPricer
    {
    public:
        GBMMonteCarloPricer(
            const Payoff& payoff,
            RandomGenerator& randomGenerator,
            const MonteCarloSettings& settings) noexcept
            : m_payoff(payoff)
            , m_randomGenerator(randomGenerator)
            , m_settings(settings)
        {
            assert(settings.RequestedTimeStep > 0.0 && "requestedTimeStep must be greater than zero");
            assert(settings.NumberOfSimulations > 0.0 && "numberOfSimulations must be greater than zero");
        }

    public:
        [[nodiscard]]
        PricingResult Price(const MarketScenario& scenario) const
        {
            GeometricBrownianMotion model(scenario.RiskFreeRate, scenario.Volatility);

            RandomGenerator randomGenerator = m_randomGenerator.Clone();

            double timeToMaturity = m_payoff.TimeToMaturity();

            SimulationGrid simulationGrid(timeToMaturity, m_settings.RequestedTimeStep);

            SimulationResult simulationResult = m_engine.Simulate(m_payoff, scenario.Spot, model, randomGenerator, simulationGrid, m_settings.NumberOfSimulations);

            double discountFactor = std::exp(-scenario.RiskFreeRate * timeToMaturity);

            PricingResult result;

            result.Value            = discountFactor * simulationResult.Mean;
            result.StandardError    = discountFactor * simulationResult.StandardError;

            return result;
        }

        [[nodiscard]]
        PricingResult Price(const MarketScenario& scenario, const Payoff& payoff) const
        {
            GBMMonteCarloPricer pricer(payoff, m_randomGenerator, m_settings);

            return pricer.Price(scenario); 
        }

    private:
        const Payoff& m_payoff;

        MonteCarloEngine<GeometricBrownianMotion> m_engine;
        RandomGenerator& m_randomGenerator;

        const MonteCarloSettings& m_settings;
        
    };
}