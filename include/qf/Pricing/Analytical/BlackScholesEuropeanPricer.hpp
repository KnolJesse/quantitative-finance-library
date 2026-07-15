#pragma once

#include "qf/Products/OptionType.hpp"
#include "qf/Pricing/PricingResult.hpp"
#include "qf/Market/MarketScenario.hpp"

#include <cassert>
#include <cmath>

namespace qf
{
    /**
     * @brief Analytical Black-Scholes pricing engine.
     *
     * Prices European options and computes their analytical Greeks
     * under the Black-Scholes model.
     *
     * Immutable after construction.  
     * All parameters are supplied to the constructor,
     * and pricing methods may be called repeatedly without modifying the 
     * object state. 
     * 
     * The model assumes:
     * - Geometric Brownian motion for the underlying asset.
     * - Constant volatility.
     * - Constant continuously compounded risk-free interest rate.
     * - No dividends.
     * - Frictionless markets.
     *
     * Supported products:
     * - European call options.
     * - European put options.
     *
     * Supported risk measures:
     * - Delta
     * - Gamma
     * - Vega
     * - Theta
     * - Rho
     */
    class BlackScholesEuropeanPricer
    {
    public:
        /**
         * @brief Constructs a Black-Scholes pricing engine.
         *
         * @param optionType Type of European option.
         * @param spot Current underlying asset price.
         * @param strike Option strike price.
         * @param riskFreeRate Continuously compounded risk-free rate.
         * @param volatility Annualized volatility expressed as a decimal.
         * @param maturity Time to expiration in years.
         * 
         * @pre spot > 0
         * @pre strike > 0
         * @pre volatility >= 0
         * @pre maturity > 0
         */
        BlackScholesEuropeanPricer(
            OptionType optionType,
            double strike,
            double maturity) noexcept
            : m_optionType(optionType)
            , m_strike(strike)
            , m_maturity(maturity)
        {
            assert(strike > 0.0 && "strike must be greater than zero");
        }

    public:
        /**
         * @brief Computes present value of the option.
         *
         * Returns the analytical Black-Scholes price corresponding to the 
         * contract and market parameters supplied at construction. 
         * 
         * @return Present value of the option.
         */
        [[nodiscard]]
        PricingResult Price(const MarketScenario& scenario) const
        { 
            switch (m_optionType)
            {
            case OptionType::Call:
                return CallPrice(scenario);

            case OptionType::Put:
                return PutPrice(scenario);
            }

            assert(false && "Option type not supported"); 

            return PricingResult{ 0.0, 0.0 };
        }

    private:
        struct BlackScholesTerms
        {
            double D1 = 0.0;
            double D2 = 0.0;
        };

    private:
        [[nodiscard]]
        static double NormalCDF(double x) noexcept
        {
            return 0.5 * std::erfc(-x / std::sqrt(2.0));
        }

        [[nodiscard]]
        BlackScholesTerms ComputeTerms(double strike, const MarketScenario& scenario) const
        {
            const double sigmaSqrtT = scenario.Volatility * std::sqrt(m_maturity);;

            BlackScholesTerms terms;

            terms.D1 =
                (std::log(scenario.Spot / m_strike)
                    +
                    (scenario.RiskFreeRate + 0.5 * scenario.Volatility * scenario.Volatility)
                    *
                    m_maturity)
                /
                sigmaSqrtT;

            terms.D2 =
                terms.D1 - sigmaSqrtT;

            return terms;
        }

        [[nodiscard]]
        PricingResult CallPrice(const MarketScenario& scenario) const
        {
            PricingResult result; 

            result.StandardError = 0.0; 

            if (scenario.Volatility == 0.0)
            {
                double terminalPrice = scenario.Spot * std::exp(scenario.RiskFreeRate * m_maturity);

                result.Value = std::exp(-scenario.RiskFreeRate * m_maturity) * std::max(terminalPrice - m_strike, 0.0);

                return result;
            }

            BlackScholesTerms terms = ComputeTerms(m_strike, scenario);

            result.Value = 
                scenario.Spot * NormalCDF(terms.D1)
                -
                m_strike * std::exp(-scenario.RiskFreeRate * m_maturity) * NormalCDF(terms.D2);

            return result; 
        }

        [[nodiscard]]
        PricingResult PutPrice(const MarketScenario& scenario) const
        {
            PricingResult result;

            result.StandardError = 0.0;

            if (scenario.Volatility == 0.0)
            {
                double terminalPrice = scenario.Spot * std::exp(scenario.RiskFreeRate * m_maturity);

                result.Value = std::exp(-scenario.RiskFreeRate * m_maturity) * std::max(m_strike - terminalPrice, 0.0);
               
                return result; 
            }

            BlackScholesTerms terms = ComputeTerms(m_strike, scenario);

            result.Value = 
                m_strike * std::exp(-scenario.RiskFreeRate * m_maturity) * NormalCDF(-terms.D2)
                -
                scenario.Spot * NormalCDF(-terms.D1);
            
            return result; 
        }

    private:
        OptionType m_optionType = OptionType::Call; 
        double m_strike = 0.0;
        double m_maturity = 0.0; 
    };
}