#pragma once

#include "qf/Payoffs/Payoff.hpp"

namespace qf
{
    class EuropeanCallPayoff : public Payoff
    {
    public:
        explicit EuropeanCallPayoff(double strike) noexcept
            : m_strike(strike)
        {
        }

    public:
        [[nodiscard]]
        double Evaluate(const Path& path) const override;

    public:
        [[nodiscard]]
        double Strike() const noexcept
        {
            return m_strike;
        }

    private:
        double m_strike = 0.0;
    };
}