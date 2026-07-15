#pragma once

#include "qf/PathGeneration/Path.hpp"

#include <memory>

namespace qf
{
    class Payoff
    {
    public:
        virtual ~Payoff() = default;

    public:
        [[nodiscard]]
        virtual double Evaluate(const Path& path) const = 0;

        [[nodiscard]]
        virtual double TimeToMaturity() const = 0;

        [[nodiscard]]
        virtual std::unique_ptr<Payoff> AdvanceTime(double dt) const = 0;
    };
}