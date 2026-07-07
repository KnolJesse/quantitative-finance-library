#pragma once

#include "qf/MonteCarlo/Path.hpp"

namespace qf
{
    class Payoff
    {
    public:
        virtual ~Payoff() = default;

    public:
        [[nodiscard]]
        virtual double Evaluate(const Path& path) const = 0;
    };
}