#pragma once

#include <cassert>
#include <cmath>

namespace qf
{
    class BlackScholesPricer
    {
    public:
        BlackScholesPricer(
            double spot,
            double strike,
            double riskFreeRate,
            double volatility,
            double maturity) noexcept
            : m_spot(spot)
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
        [[nodiscard]]
        double Price() const
        {
            if (m_volatility == 0.0)
            {
                const double terminalPrice = m_spot * std::exp(m_riskFreeRate * m_maturity);

                return std::exp(-m_riskFreeRate * m_maturity) * std::max(terminalPrice - m_strike, 0.0);
            }

            const double sqrtT      = std::sqrt(m_maturity);
            const double sigmaSqrtT = m_volatility * sqrtT;

            const double d1 = 
                (std::log(m_spot / m_strike)
                +
                (m_riskFreeRate + 0.5 * m_volatility * m_volatility)
                *
                m_maturity)
                /
                sigmaSqrtT;

            const double d2 = 
                d1 - sigmaSqrtT;

            return
                m_spot * NormalCDF(d1)
                -
                m_strike * std::exp(-m_riskFreeRate * m_maturity) * NormalCDF(d2);
        }

    private:
        [[nodiscard]]
        static double NormalCDF(double x) noexcept
        {
            return 0.5 * std::erfc(-x / std::sqrt(2.0));
        }

    private:
        double m_spot = 0.0;
        double m_strike = 0.0;
        double m_riskFreeRate = 0.0;
        double m_volatility = 0.0;
        double m_maturity = 0.0;
    };
}