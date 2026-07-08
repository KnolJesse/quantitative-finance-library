#pragma once 

#include <cstddef>
#include <utility>
#include <vector>

namespace qf
{
	class Path 
	{
	public:
		Path() = default; 

		explicit Path(std::vector<double> values) noexcept
			: m_values(std::move(values)) 
		{
		}

	public:
		[[nodiscard]]
		std::size_t Size() const noexcept
		{
			return m_values.size();
		}

		[[nodiscard]]
		double At(std::size_t index) const
		{
			return m_values.at(index); 
		}

		[[nodiscard]]
		double Front() const
		{
			return m_values.front(); 
		}

		[[nodiscard]]
		double Back() const
		{
			return m_values.back(); 
		}

		[[nodiscard]]
		const std::vector<double>& Values() const noexcept
		{
			return m_values; 
		}

	private:
		std::vector<double> m_values;
	};
 
}