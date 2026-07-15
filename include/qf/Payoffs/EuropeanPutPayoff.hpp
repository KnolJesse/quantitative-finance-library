#pragma once

#include "qf/Payoffs/Payoff.hpp"
#include "qf/PathGeneration/Path.hpp"

#include <cassert>

namespace qf
{
    class EuropeanPutPayoff final : public Payoff
    {
    public:
        explicit EuropeanPutPayoff(double strike, double maturity) noexcept
            : m_strike(strike)
            , m_maturity(maturity)
        {
            assert(strike > 0.0 && "strike must be greater than zero");
            assert(maturity > 0.0 && "maturity must be greater than zero");
        }

    public:
        [[nodiscard]]
        double Evaluate(const Path& path) const override;

        [[nodiscard]]
        double TimeToMaturity() const override;

        [[nodiscard]]
        std::unique_ptr<Payoff> AdvanceTime(double dt) const override;

    public:
        [[nodiscard]]
        double Strike() const noexcept
        {
            return m_strike;
        }

    private:
        double m_strike = 0.0;
        double m_maturity = 0.0;
    };
}