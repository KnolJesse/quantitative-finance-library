#include "qf/Payoffs/EuropeanCallPayoff.hpp"

#include <algorithm>
#include <cassert>

namespace qf
{
	double EuropeanCallPayoff::Evaluate(const Path& path) const
	{
		assert(path.Size() > 0 && "path must contain at least one value.");

		const double terminalPrice = path.Back();

		return std::max(terminalPrice - m_strike, 0.0); 
	}
}