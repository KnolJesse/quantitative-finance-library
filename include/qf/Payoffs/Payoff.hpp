#pragma once

#include "qf/PathGeneration/Path.hpp"

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