#include "qf/Payoffs/EuropeanCallPayoff.hpp"
#include "qf/Payoffs/Payoff.hpp"

#include <algorithm>
#include <cassert>

namespace qf
{
	double EuropeanCallPayoff::Evaluate(const Path& path) const
	{
		assert(!path.empty() && "path must contain at least one value.");

		const double terminalPrice = path.back();

		return std::max(terminalPrice - m_strike, 0.0); 
	}
}