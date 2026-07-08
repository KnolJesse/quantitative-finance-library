#pragma once

#include "qf/Pricing/PricingResult.hpp"
#include "qf/MonteCarlo/PathGenerator.hpp"
#include "qf/Payoffs/Payoff.hpp"
#include "qf/Statistics/RunningStats.hpp"

#include <cstddef>

namespace qf
{
    template<typename Model>
    class MonteCarloPricer
    {
    public:
        MonteCarloPricer(
            PathGenerator<Model>& pathGenerator,
            const Payoff& payoff,
            double riskFreeRate,
            double maturity,
            std::size_t simulations) noexcept
            : m_pathGenerator(pathGenerator)
            , m_payoff(payoff)
            , m_riskFreeRate(riskFreeRate)
            , m_maturity(maturity)
            , m_simulations(simulations)
        {
            assert(riskFreeRate >= 0.0 && "riskFreeRate must be non-negative");
            assert(maturity > 0.0 && "maturity must be greater than zero");
            assert(simulations > 0 && "simulations must be greater than zero");
        }

    public:
        [[nodiscard]]
        PricingResult Price()
        {
            RunningStats statistics; 

            for (std::size_t s = 0; s < m_simulations; ++s)
            {
                const Path path = m_pathGenerator.Generate();

                const double payoff = m_payoff.Evaluate(path);

                statistics.Add(payoff);
            }

            const double discountFactor = std::exp(-m_riskFreeRate * m_maturity); 

            PricingResult result; 
            
            result.Value = discountFactor * statistics.Mean();
            result.StandardError = discountFactor * std::sqrt(statistics.Variance() / static_cast<double>(m_simulations)); 

            return result; 
        }

    private:
        PathGenerator<Model>& m_pathGenerator;
        const Payoff& m_payoff;

        double m_riskFreeRate = 0.0;
        double m_maturity = 0.0;

        std::size_t m_simulations = 0;
    };
}