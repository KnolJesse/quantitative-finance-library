#include "qf/Payoffs/EuropeanCallPayoff.hpp"

#include <cassert>

namespace qf
{
	double EuropeanCallPayoff::Evaluate(const Path& path) const
	{
		assert(!path.empty() && "path must contain at least one value.");

		const double terminalPrice = path.valueAt(m_maturity);

		return std::max(terminalPrice - m_strike, 0.0); 
	}

	[[nodiscard]]
	double EuropeanCallPayoff::TimeToMaturity() const
	{
		return m_maturity;
	}

	[[nodiscard]]
	std::unique_ptr<Payoff> EuropeanCallPayoff::AdvanceTime(double dt) const
	{
		return std::make_unique<EuropeanCallPayoff>(
			m_strike,
			m_maturity - dt
		);
	}
}