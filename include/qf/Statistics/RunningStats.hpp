#pragma once

#include <cstddef>
#include <limits>

namespace qf
{
	class RunningStats
	{
	public:
		RunningStats() = default; 

	public:
		void Add(double value) noexcept; 

	public:
		[[nodiscard]]
		std::size_t Count() const noexcept
		{
			return m_count;
		}

		[[nodiscard]]
		double Mean() const noexcept
		{
			if (m_count == 0)
			{
				return std::numeric_limits<double>::quiet_NaN(); 
			}

			return m_mean; 
		}

		[[nodiscard]]
		double Variance() const noexcept
		{
			if (m_count < 2)
			{
				return std::numeric_limits<double>::quiet_NaN(); 
			}

			return m_M2 / static_cast<double>(m_count - 1); 
		}

		[[nodiscard]]
		double PopulationVariance() const noexcept
		{
			if (m_count == 0)
			{
				return std::numeric_limits<double>::quiet_NaN();
			}

			return m_M2 / static_cast<double>(m_count); 
		}

		[[nodiscard]]
		bool IsEmpty() const noexcept
		{
			return m_count == 0; 
		}
	
	private: 
		std::size_t m_count = 0; 
		double m_mean = 0.0; 
		double m_M2 = 0.0; 
	};
}
