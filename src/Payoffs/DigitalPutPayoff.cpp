#include "qf/Payoffs/DigitalPutPayoff.hpp"

#include <cassert> 

namespace qf
{
    double DigitalPutPayoff::Evaluate(const Path& path) const
    {
        assert(!path.empty() && "path must contain at least one value");

        const double terminalPrice = path.back();

        return (terminalPrice < m_strike) ? m_payout : 0.0;
    }
}