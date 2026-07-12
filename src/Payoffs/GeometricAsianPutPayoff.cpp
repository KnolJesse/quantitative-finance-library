#include "qf/Payoffs/GeometricAsianPutPayoff.hpp"

#include <cmath>
#include <cassert>

namespace qf
{
	double GeometricAsianPutPayoff::Evaluate(const Path& path) const
	{
		assert(!path.empty() && "path must contain at least one value.");

		double sumLogPrices = 0.0;

		for (double observationTime : m_observationSchedule)
		{
			sumLogPrices += std::log(path.valueAt(observationTime));
		}

		const double geometricAverage = std::exp(sumLogPrices / static_cast<double>(m_observationSchedule.size()));

		return std::max(m_strike - geometricAverage, 0.0);
	}
}