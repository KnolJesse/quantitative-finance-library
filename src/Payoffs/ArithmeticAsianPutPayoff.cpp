#include "qf/Payoffs/ArithmeticAsianPutPayoff.hpp"

#include <cassert>

namespace qf
{
	double ArithmeticAsianPutPayoff::Evaluate(const Path& path) const
	{
		assert(!path.empty() && "path must contain at least one value.");

		double sum = 0.0;

		for (double price : path)
		{
			sum += price;
		}

		const double averagePrice = sum / static_cast<double>(path.size());

		return std::max(m_strike - averagePrice, 0.0);
	}
}