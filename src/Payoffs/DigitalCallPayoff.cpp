#include "qf/Payoffs/DigitalCallPayoff.hpp"

#include <cassert>

namespace qf
{
    double DigitalCallPayoff::Evaluate(const Path& path) const
    {
        assert(!path.empty() && "path must contain at least one value");

        const double terminalPrice = path.valueAt(m_maturity);

        return (terminalPrice > m_strike) ? m_payout : 0.0;
    }

	[[nodiscard]]
	double DigitalCallPayoff::TimeToMaturity() const
	{
		return m_maturity;
	}

	[[nodiscard]]
	std::unique_ptr<Payoff> DigitalCallPayoff::AdvanceTime(double dt) const
	{
		return std::make_unique<DigitalCallPayoff>(
			m_strike,
			m_maturity - dt,
			m_payout
		);
	}
}