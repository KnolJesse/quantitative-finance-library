#pragma once

#include "qf/Payoffs/Payoff.hpp"
#include "qf/PathGeneration/Path.hpp"

#include <cassert>

namespace qf
{
    class EuropeanPutPayoff final : public Payoff
    {
    public:
        explicit EuropeanPutPayoff(double strike) noexcept
            : m_strike(strike)
        {
            assert(strike > 0.0 && "strike must be greater than zero");
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