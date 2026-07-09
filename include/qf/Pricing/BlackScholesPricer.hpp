#pragma once

#include "qf/Products/OptionType.hpp"

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
    class BlackScholesPricer
    {
    public:
        /**
         * @brief Constructs a Black-Scholes pricing engine.
         *
         * @param optionType Type of European option.
         * @param spot Current underlying asset price.
         * @param strike Option strike price.
         * @param riskFreeRate Continuously compounded risk-free rate.
         * @param volatility Annualized volatility.
         * @param maturity Time to expiration in years.
         */
        BlackScholesPricer(
            OptionType optionType,
            double spot,
            double strike,
            double riskFreeRate,
            double volatility,
            double maturity) noexcept
            : m_optionType(optionType)
            , m_spot(spot)
            , m_strike(strike)
            , m_riskFreeRate(riskFreeRate)
            , m_volatility(volatility)
            , m_maturity(maturity)
        {
            assert(spot > 0.0 && "spot must be greater than zero");
            assert(strike > 0.0 && "strike must be greater than zero");
            assert(volatility >= 0.0 && "volatility must be non-negative");
            assert(maturity > 0.0 && "maturity must be greater than zero");
        }

    public:
        /**
         * @brief Computes the option price.
         *
         * @return Present value of the option.
         */
        [[nodiscard]]
        double Price() const
        {
            switch (m_optionType)
            {
            case OptionType::Call:
                return CallPrice();

            case OptionType::Put:
                return PutPrice();
            }

            assert(false && "Option type not supported"); 

            return 0.0;
        }

        /**
         * @brief Computes option Delta.
         *
         * Delta measures the sensitivity of option value
         * with respect to changes in the underlying asset price.
         *
         * @return First derivative of price with respect to spot.
         */
        [[nodiscard]]
        double Delta() const
        {
            if (m_volatility == 0.0)
            {
                const double terminalPrice = m_spot * std::exp(m_riskFreeRate * m_maturity);

                // Delta = 1 if option is guaranteed to expire in the money.
                // Delta = 0 otherwise.
                return (terminalPrice > m_strike) ? 1.0 : 0.0;
            }

            const BlackScholesTerms terms = ComputeTerms();

            switch (m_optionType)
            {
            case OptionType::Call:
                return NormalCDF(terms.D1);

            case OptionType::Put:
                return NormalCDF(terms.D1) - 1.0;
            }

            assert(false && "Option type not supported");

            return 0.0;
        }

        /**
         * @brief Computes option Gamma.
         *
         * Gamma measures the sensitivity of Delta with respect to
         * changes in the underlying asset price. It is the second
         * derivative of the option value with respect to spot.
         *
         * @return Second derivative of price with respect to spot.
         */
        [[nodiscard]]
        double Gamma() const
        {
            if (m_volatility == 0.0)
            {
                return 0.0;
            }

            const BlackScholesTerms terms = ComputeTerms();

            return NormalPDF(terms.D1) / (m_spot * m_volatility * std::sqrt(m_maturity));
        }

        /**
         * @brief Computes option Vega.
         *
         * Vega measures the sensitivity of the option value with
         * respect to changes in the volatility of the underlying asset.
         *
         * Vega is quoted per unit volatility (e.g. a change from
         * 20% to 21% corresponds to a volatility increase of 0.01).
         *
         * @return First derivative of price with respect to volatility.
         */
        [[nodiscard]]
        double Vega() const
        {
            if (m_volatility == 0.0)
            {
                return 0.0;
            }

            const BlackScholesTerms terms = ComputeTerms();

            return m_spot * NormalPDF(terms.D1) * std::sqrt(m_maturity);
        }

        /**
         * @brief Computes option Theta.
         *
         * Theta measures the sensitivity of the option value with
         * respect to the passage of time while all other parameters
         * remain constant.
         *
         * Theta is quoted per year.
         *
         * @return First derivative of price with respect to time to maturity.
         */
        [[nodiscard]]
        double Theta() const
        {
            if (m_volatility == 0.0)
            {
                return 0.0;
            }

            const BlackScholesTerms terms = ComputeTerms();

            const double firstThetaTerm =
                -m_spot * NormalPDF(terms.D1) * m_volatility
                /
                (2.0 * std::sqrt(m_maturity)); 

            switch (m_optionType)
            {
            case OptionType::Call:
                return firstThetaTerm 
                    - 
                    m_riskFreeRate * m_strike * std::exp(-m_riskFreeRate * m_maturity)
                    *
                    NormalCDF(terms.D2); 
            
            case OptionType::Put:
                return firstThetaTerm 
                    +
                    m_riskFreeRate * m_strike * std::exp(-m_riskFreeRate * m_maturity)
                    *
                    NormalCDF(-terms.D2);
            }

            assert(false && "Option type not supported");

            return 0.0; 
        }

        /**
         * @brief Computes option Rho.
         *
         * Rho measures the sensitivity of the option value with
         * respect to changes in the continuously compounded
         * risk-free interest rate.
         *
         * Rho is quoted per unit interest rate (e.g. a change
         * from 5% to 6% corresponds to an interest rate increase of 0.01).
         *
         * @return First derivative of price with respect to the risk-free rate.
         */
        [[nodiscard]]
        double Rho() const
        {
            if (m_volatility == 0.0)
            {
                return 0.0;
            }

            const BlackScholesTerms terms = ComputeTerms();

            switch (m_optionType)
            {
            case OptionType::Call:
                return 
                    m_strike * m_maturity * std::exp(-m_riskFreeRate * m_maturity)
                    *
                    NormalCDF(terms.D2);

            case OptionType::Put:
                return 
                    -m_strike * m_maturity * std::exp(-m_riskFreeRate * m_maturity)
                    *
                    NormalCDF(-terms.D2);
            }

            assert(false && "Option type not supported");

            return 0.0;
        }

    private:
        struct BlackScholesTerms
        {
            double D1 = 0.0;
            double D2 = 0.0;
        };

    private:
        [[nodiscard]]
        double CallPrice() const
        {
            if (m_volatility == 0.0)
            {
                const double terminalPrice = m_spot * std::exp(m_riskFreeRate * m_maturity);

                return std::exp(-m_riskFreeRate * m_maturity) * std::max(terminalPrice - m_strike, 0.0);
            }

            const BlackScholesTerms terms = ComputeTerms();

            return
                m_spot * NormalCDF(terms.D1)
                -
                m_strike * std::exp(-m_riskFreeRate * m_maturity) * NormalCDF(terms.D2);
        }

        [[nodiscard]]
        double PutPrice() const
        {
            if (m_volatility == 0.0)
            {
                const double terminalPrice = m_spot * std::exp(m_riskFreeRate * m_maturity);

                return std::exp(-m_riskFreeRate * m_maturity) * std::max(m_strike - terminalPrice, 0.0);
            }

            const BlackScholesTerms terms = ComputeTerms();

            return
                m_strike * std::exp(-m_riskFreeRate * m_maturity) * NormalCDF(-terms.D2)
                -
                m_spot * NormalCDF(-terms.D1); 
        }

        [[nodiscard]]
        static double NormalCDF(double x) noexcept
        {
            return 0.5 * std::erfc(-x / std::sqrt(2.0));
        }

        [[nodiscard]]
        static double NormalPDF(double x) noexcept
        {
            constexpr double inverseSqrtTwoPi = 0.39894228040143267793994605993438; 

            return inverseSqrtTwoPi * std::exp(-0.5 * x * x); 
        }

        [[nodiscard]]
        BlackScholesTerms ComputeTerms() const noexcept
        {
            const double sqrtT = std::sqrt(m_maturity);
            const double sigmaSqrtT = m_volatility * sqrtT; 

            BlackScholesTerms terms; 

            terms.D1 =
                (std::log(m_spot / m_strike)
                +
                (m_riskFreeRate + 0.5 * m_volatility * m_volatility)
                *
                m_maturity)
                /
                sigmaSqrtT; 

            terms.D2 =
                terms.D1 - sigmaSqrtT; 

            return terms; 
        }

    private:
        OptionType m_optionType = OptionType::Call; 
        double m_spot = 0.0;
        double m_strike = 0.0;
        double m_riskFreeRate = 0.0;
        double m_volatility = 0.0;
        double m_maturity = 0.0;
    };
}