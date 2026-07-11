#pragma once

#include "qf/Payoffs/Payoff.hpp"
#include "qf/PathGeneration/Path.hpp"

#include <cassert>

namespace qf
{
    class DigitalCallPayoff final : public Payoff
    {
    public:
        explicit DigitalCallPayoff(double strike, double payout = 1.0) noexcept
            : m_strike(strike)
            , m_payout(payout)
        {
            assert(strike > 0.0 && "strike must be greater than zero");
            assert(payout > 0.0 && "payout must be greater than zero");
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

        [[nodiscard]]
        double Payout() const noexcept
        {
            return m_payout;
        }

    private:
        double m_strike = 0.0;
        double m_payout = 0.0;
    };
}