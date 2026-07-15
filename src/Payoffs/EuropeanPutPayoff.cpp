#include "qf/Payoffs/EuropeanPutPayoff.hpp"

#include <cassert>

namespace qf
{
	double EuropeanPutPayoff::Evaluate(const Path& path) const
	{
		assert(path.size() > 0 && "path must contain at least one value.");

		const double terminalPrice = path.valueAt(m_maturity);

		return std::max(m_strike - terminalPrice, 0.0);
	}

	[[nodiscard]]
	double EuropeanPutPayoff::TimeToMaturity() const
	{
		return m_maturity;
	}

	[[nodiscard]]
	std::unique_ptr<Payoff> EuropeanPutPayoff::AdvanceTime(double dt) const
	{
		return std::make_unique<EuropeanPutPayoff>(
			m_strike,
			m_maturity - dt
		);
	}
}