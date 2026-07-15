#include "qf/Payoffs/GeometricAsianCallPayoff.hpp"

#include <cmath>
#include <cassert>

namespace qf
{
	double GeometricAsianCallPayoff::Evaluate(const Path& path) const
	{
		assert(!path.empty() && "path must contain at least one value.");

		double sumLogPrices = 0.0;

		for (double observationTime : m_observationSchedule)
		{
			sumLogPrices += std::log(path.valueAt(observationTime));
		}

		const double geometricAverage = std::exp(sumLogPrices / static_cast<double>(m_observationSchedule.size()));

		return std::max(geometricAverage - m_strike, 0.0);
	}

	[[nodiscard]]
	double GeometricAsianCallPayoff::TimeToMaturity() const
	{
		return m_maturity;
	}

	[[nodiscard]]
	std::unique_ptr<Payoff> GeometricAsianCallPayoff::AdvanceTime(double dt) const
	{
		ObservationSchedule advancedSchedule = m_observationSchedule.AdvanceBy(dt);

		return std::make_unique<GeometricAsianCallPayoff>(
			m_strike,
			m_maturity - dt,
			advancedSchedule
		);
	}
}