#include "qf/Payoffs/DigitalPutPayoff.hpp"

#include <cassert> 

namespace qf
{
    double DigitalPutPayoff::Evaluate(const Path& path) const
    {
        assert(!path.empty() && "path must contain at least one value");

        const double terminalPrice = path.valueAt(m_maturity);

        return (terminalPrice < m_strike) ? m_payout : 0.0;
    }

	[[nodiscard]]
	double DigitalPutPayoff::TimeToMaturity() const
	{
		return m_maturity;
	}

	[[nodiscard]]
	std::unique_ptr<Payoff> DigitalPutPayoff::AdvanceTime(double dt) const
	{
		return std::make_unique<DigitalPutPayoff>(
			m_strike,
			m_maturity - dt,
			m_payout
		);
	}
}