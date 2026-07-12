#pragma once

#include "qf/Products/OptionType.hpp"

#include <cassert>
#include <cstddef>
#include <cmath>
#include <algorithm>

namespace qf
{
    class BlackScholesGeometricAsianPricer
    {
    public:
        BlackScholesGeometricAsianPricer(
            OptionType optionType,
            double spot,
            double strike,
            double riskFreeRate,
            double volatility,
            double maturity,
            std::size_t numberOfObservations) noexcept
            : m_optionType(optionType)
            , m_spot(spot)
            , m_strike(strike)
            , m_riskFreeRate(riskFreeRate)
            , m_volatility(volatility)
            , m_maturity(maturity)
            , m_numberOfObservations(numberOfObservations)
            , m_effectiveVolatility(
                EffectiveVolatility(
                    m_volatility, 
                    m_numberOfObservations))
            , m_effectiveDrift(
                EffectiveDrift(
                    m_volatility, 
                    m_effectiveVolatility,
                    m_riskFreeRate,
                    m_numberOfObservations))
            , m_averageObservationTime(
                AverageObservationTime(
                    m_maturity,
                    m_numberOfObservations))
        {
            assert(spot > 0.0 && "spot must be greater than zero");
            assert(strike > 0.0 && "strike must be greater than zero");
            assert(volatility >= 0.0 && "volatility must be non-negative");
            assert(maturity > 0.0 && "maturity must be greater than zero");
            assert(numberOfObservations > 0 && "numberOfObservations must be greater than zero");
        }

    public:
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
                const double geometricAverage = m_spot * std::exp(m_riskFreeRate * m_averageObservationTime);

                return std::exp(-m_riskFreeRate * m_maturity) * std::max(geometricAverage - m_strike, 0.0); 
            }

            const BlackScholesTerms terms = ComputeTerms();

            return
                std::exp(-m_riskFreeRate * m_maturity)
                *
                (m_spot * std::exp(m_effectiveDrift * m_maturity) * NormalCDF(terms.D1)
                -
                m_strike * NormalCDF(terms.D2));
        }

        [[nodiscard]]
        double PutPrice() const
        {
            if (m_volatility == 0.0)
            {
                const double geometricAverage = m_spot * std::exp(m_riskFreeRate * m_averageObservationTime);

                return exp(-m_riskFreeRate * m_maturity) * std::max(m_strike - geometricAverage, 0.0);
            }

            const BlackScholesTerms terms = ComputeTerms();

            return
                std::exp(-m_riskFreeRate * m_maturity)
                *
                (m_strike * NormalCDF(-terms.D2)
                -
                m_spot * std::exp(m_effectiveDrift * m_maturity) * NormalCDF(-terms.D1));
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
        static double EffectiveVolatility(double volatility, std::size_t numberOfObservations) noexcept
        {
            const double n = static_cast<double>(numberOfObservations); 

            const double term1 = (n + 1.0) * (2.0 * n + 1.0);
            const double term2 = 6.0 * n * n; 

            return volatility * std::sqrt(term1 / term2); 
        }

        [[nodiscard]]
        static double EffectiveDrift(
            double volatility,
            double effectiveVolatility,
            double riskFreeRate,
            std::size_t numberOfObservations) noexcept
        {
            const double n = static_cast<double>(numberOfObservations);

            const double term1 = (riskFreeRate - 0.5 * volatility * volatility);
            const double term2 = (n + 1.0) / (2.0 * n); 
            const double term3 = 0.5 * effectiveVolatility * effectiveVolatility; 

            return term1 * term2 + term3; 
        }

        [[nodiscard]]
        static double AverageObservationTime(double maturity, std::size_t numberOfObservations) noexcept 
        {
            const double n = static_cast<double>(numberOfObservations);

            return
                maturity * (n + 1.0)
                /
                (2.0 * n);
        }

        [[nodiscard]]
        BlackScholesTerms ComputeTerms() const noexcept
        {
            BlackScholesTerms geometricAsianTerms;

            const double sigmaSqrtT = m_effectiveVolatility * std::sqrt(m_maturity); 

            const double term1 = std::log(m_spot / m_strike);
            const double term2 = (m_effectiveDrift + 0.5 * m_effectiveVolatility * m_effectiveVolatility) * m_maturity;  

            geometricAsianTerms.D1 = (term1 + term2) / sigmaSqrtT; 
            geometricAsianTerms.D2 = geometricAsianTerms.D1 - sigmaSqrtT; 
           
            return geometricAsianTerms;
        }

    private:
        OptionType m_optionType = OptionType::Call;

        double m_spot = 0.0;
        double m_strike = 0.0;
        double m_riskFreeRate = 0.0;
        double m_volatility = 0.0;
        double m_maturity = 0.0;

        std::size_t m_numberOfObservations = 0;

        const double m_effectiveVolatility = 0.0; 
        const double m_effectiveDrift = 0.0; 

        const double m_averageObservationTime = 0.0; 
    };
}