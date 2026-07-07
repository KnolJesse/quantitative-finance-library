#include "qf/Payoffs/EuropeanPutPayoff.hpp"
#include "qf/Payoffs/Payoff.hpp"

#include <algorithm>
#include <cassert>

namespace qf
{
	double EuropeanPutPayoff::Evaluate(const Path& path) const
	{
		assert(path.Size() > 0 && "path must contain at least one value.");

		const double terminalPrice = path.Back();

		return std::max(m_strike - terminalPrice, 0.0);
	}
}