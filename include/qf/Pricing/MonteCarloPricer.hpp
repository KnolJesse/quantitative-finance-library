#pragma once

#include "qf/PathGeneration/PathGenerator.hpp"
#include "qf/Payoffs/Payoff.hpp"
#include "qf/Pricing/PricingResult.hpp"
#include "qf/Random/RandomGenerator.hpp"
#include "qf/Statistics/RunningStats.hpp"

#include <cstddef>
#include <cmath>
#include <cassert>

namespace qf
{
    template<typename Model>
    class MonteCarloPricer
    {
    public:
        MonteCarloPricer(
            const Model& model,
            RandomGenerator& randomGenerator,
            const Payoff& payoff,
            double spot,
            double riskFreeRate,
            double maturity,
            double timeStep,
            std::size_t numberOfSteps,
            std::size_t simulations) noexcept
            : m_model(model)
            , m_randomGenerator(randomGenerator)
            , m_payoff(payoff)
            , m_spot(spot)
            , m_riskFreeRate(riskFreeRate)
            , m_maturity(maturity)
            , m_timeStep(timeStep)
            , m_numberOfSteps(numberOfSteps)
            , m_simulations(simulations)
        {
            assert(riskFreeRate >= 0.0 &&
                "riskFreeRate must be non-negative");

            assert(maturity > 0.0 &&
                "maturity must be greater than zero");

            assert(timeStep > 0.0 &&
                "timeStep must be greater than zero");

            assert(numberOfSteps > 0 &&
                "numberOfSteps must be greater than zero");

            assert(simulations > 0 &&
                "simulations must be greater than zero");
        }

    public:
        [[nodiscard]]
        PricingResult Price() const
        {
            RandomGenerator randomGenerator = m_randomGenerator.Clone();

            return PriceImplementation(
                m_model,
                randomGenerator,
                m_spot,
                m_riskFreeRate,
                m_maturity
            );
        }

        [[nodiscard]]
        double Delta(double bump)
        {
            const PricingResult up = PriceScenario(
                m_model,
                m_spot + bump,
                m_riskFreeRate,
                m_maturity
            );

            const PricingResult down = PriceScenario(
                m_model,
                m_spot - bump,
                m_riskFreeRate,
                m_maturity
            ); 

            return
                (up.Value - down.Value)
                /
                (2.0 * bump); 
        }

        [[nodiscard]]
        double Gamma(double bump)
        {
            const PricingResult up = PriceScenario(
                m_model,
                m_spot + bump,
                m_riskFreeRate,
                m_maturity
            );

            const PricingResult middle = PriceScenario(
                m_model,
                m_spot,
                m_riskFreeRate,
                m_maturity
            );

            const PricingResult down = PriceScenario(
                m_model,
                m_spot - bump,
                m_riskFreeRate,
                m_maturity
            );

            return
                (up.Value - 2.0 * middle.Value + down.Value)
                /
                (bump * bump);
        }

        [[nodiscard]]
        double Vega(double bump) const
        {
            Model upModel(m_model.Drift(), m_model.Volatility() + bump);
            Model downModel(m_model.Drift(), m_model.Volatility() - bump);

            const PricingResult up = PriceScenario(
                upModel,
                m_spot,
                m_riskFreeRate,
                m_maturity
            ); 

            const PricingResult down = PriceScenario(
                downModel,
                m_spot,
                m_riskFreeRate,
                m_maturity
            );

            return
                (up.Value - down.Value)
                /
                (2.0 * bump);
        }

        [[nodiscard]]
        double Rho(double bump) const
        {
            Model upModel(m_riskFreeRate + bump, m_model.Volatility());
            Model downModel(m_riskFreeRate - bump, m_model.Volatility());

            const PricingResult up = PriceScenario(
                upModel,
                m_spot,
                m_riskFreeRate + bump,
                m_maturity
            ); 

            const PricingResult down = PriceScenario(
                downModel,
                m_spot,
                m_riskFreeRate - bump,
                m_maturity
            );

            return
                (up.Value - down.Value)
                /
                (2.0 * bump);
        }

        [[nodiscard]]
        double Theta(double bump) const
        {
            const PricingResult shorter = PriceScenario(
                m_model,
                m_spot,
                m_riskFreeRate,
                m_maturity - bump
            ); 

            const PricingResult longer = PriceScenario(
                m_model,
                m_spot,
                m_riskFreeRate,
                m_maturity + bump
            );

            return
                (shorter.Value - longer.Value)
                /
                (2.0 * bump); 
        }


    private:
        [[nodiscard]]
        PricingResult PriceScenario(
            const Model& model,
            double spot,
            double riskFreeRate,
            double maturity) const
        {
            RandomGenerator randomGenerator = m_randomGenerator.Clone(); 

            return PriceImplementation(
                model,
                randomGenerator,
                spot,
                riskFreeRate,
                maturity
            );
        }

        [[nodiscard]]
        PricingResult PriceImplementation(
            const Model& model,
            RandomGenerator& randomGenerator,
            double spot,
            double riskFreeRate,
            double maturity) const
        {
            RunningStats statistics;

            PathGenerator<Model> pathGenerator(model, randomGenerator);

            for (std::size_t s = 0; s < m_simulations; ++s)
            {
                const Path path = pathGenerator.Generate(spot, m_timeStep, m_numberOfSteps);

                const double payoff = m_payoff.Evaluate(path);

                statistics.Add(payoff);
            }

            const double discountFactor = std::exp(-riskFreeRate * maturity);

            PricingResult result;

            result.Value = discountFactor * statistics.Mean(); 

            result.StandardError = 
                discountFactor 
                * 
                std::sqrt(statistics.Variance() / static_cast<double>(m_simulations));
                

            return result;
        }

    private:
        const Model& m_model;
        RandomGenerator& m_randomGenerator;

        const Payoff& m_payoff;

        double m_spot = 0.0;
        double m_riskFreeRate = 0.0;
        double m_maturity = 0.0;
        double m_timeStep = 0.0;

        std::size_t m_numberOfSteps = 0;
        std::size_t m_simulations = 0;
    };
}