#include "qf/Payoffs/ArithmeticAsianCallPayoff.hpp"

#include <cassert>

namespace qf
{
	double ArithmeticAsianCallPayoff::Evaluate(const Path& path) const
	{
		assert(!path.empty() && "path must contain at least one value.");

		double sum = 0.0;

		for (double observationTime : m_observationSchedule)
		{
			sum += path.valueAt(observationTime);
		}

		const double averagePrice = sum / static_cast<double>(m_observationSchedule.size());

		return std::max(averagePrice - m_strike, 0.0);
	}
}