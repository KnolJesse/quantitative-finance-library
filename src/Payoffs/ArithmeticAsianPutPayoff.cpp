#include "qf/Payoffs/ArithmeticAsianPutPayoff.hpp"

#include <cassert>

namespace qf
{
	double ArithmeticAsianPutPayoff::Evaluate(const Path& path) const
	{
		assert(!path.empty() && "path must contain at least one value.");

		double sum = 0.0;

		for (double observationTime : m_observationSchedule)
		{
			sum += path.valueAt(observationTime);
		}

		const double averagePrice = sum / static_cast<double>(m_observationSchedule.size());

		return std::max(m_strike - averagePrice, 0.0);
	}

	[[nodiscard]]
	double ArithmeticAsianPutPayoff::TimeToMaturity() const
	{
		return m_maturity;
	}

	[[nodiscard]]
	std::unique_ptr<Payoff> ArithmeticAsianPutPayoff::AdvanceTime(double dt) const
	{
		ObservationSchedule advancedSchedule = m_observationSchedule.AdvanceBy(dt);

		return std::make_unique<ArithmeticAsianPutPayoff>(
			m_strike,
			m_maturity - dt,
			advancedSchedule
		);
	}
}