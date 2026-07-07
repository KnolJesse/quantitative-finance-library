#include "qf/Statistics/RunningStats.hpp"

namespace qf
{
	void RunningStats::Add(double value) noexcept
	{
		++m_count;

		if (m_count == 1)
		{
			m_mean = value;
			m_M2 = 0.0;
			return;
		}

		const double delta = value - m_mean;
		m_mean += delta / static_cast<double>(m_count);

		const double delta2 = value - m_mean;
		m_M2 += delta * delta2;
	}
}