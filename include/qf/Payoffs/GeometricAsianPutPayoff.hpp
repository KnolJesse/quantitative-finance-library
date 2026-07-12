#pragma once

#include "qf/Payoffs/Payoff.hpp"
#include "qf/PathGeneration/Path.hpp"
#include "qf/Time/ObservationSchedule.hpp"

#include <cassert>

namespace qf
{
    class GeometricAsianPutPayoff final : public Payoff
    {
    public:
        explicit GeometricAsianPutPayoff(
            double strike,
            ObservationSchedule observationSchedule) noexcept
            : m_strike(strike)
            , m_observationSchedule(std::move(observationSchedule))
        {
            assert(m_strike > 0.0 && "strike must be greater than zero");
            assert(!m_observationSchedule.empty() && "schedule must not be empty");
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
        ObservationSchedule Schedule() const
        {
            return m_observationSchedule;
        }

    private:
        double m_strike = 0.0;
        ObservationSchedule m_observationSchedule;
    };
}