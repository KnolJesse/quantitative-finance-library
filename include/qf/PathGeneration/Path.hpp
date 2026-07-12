#pragma once

#include <cassert>
#include <cmath>
#include <cstddef>
#include <utility>
#include <vector>
#include <algorithm>

namespace qf
{
    class Path
    {
    public:
        using value_type = double;
        using size_type = std::size_t;
        using const_iterator = std::vector<double>::const_iterator;

    public:
        Path() = default;

        explicit Path(std::vector<double> times, std::vector<double> values)
            : m_times(std::move(times))
            , m_values(std::move(values))
        {
            Validate(); 
        }

        Path(std::initializer_list<double> times, std::initializer_list<double> values)
            : m_times(times)
            , m_values(values)
        {
            Validate(); 
        }

    public:
        [[nodiscard]]
        size_type size() const noexcept
        {
            return m_values.size();
        }

        [[nodiscard]]
        bool empty() const noexcept
        {
            return m_values.empty();
        }

        [[nodiscard]]
        value_type at(size_type index) const
        {
            return m_values.at(index);
        }

        [[nodiscard]]
        value_type valueAt(double time) const
        {
            assert(time >= m_times.front() && "Requested time is before the start of the path");
            assert(time <= m_times.back() && "Requested time is after the end of the path");
            
            const auto iterator = std::lower_bound(m_times.begin(), m_times.end(), time); 

            assert(iterator != m_times.end() && "Requested time was not found");

            const std::size_t index = static_cast<std::size_t>(std::distance(m_times.begin(), iterator));

            const double tolerance = 1e-12; 

            assert(std::abs(*iterator - time) < tolerance && "Requested time not in path");

            return m_values[index]; 
        }

        [[nodiscard]]
        value_type timeAt(size_type index) const
        {
            return m_times.at(index);
        }

        [[nodiscard]]
        const std::vector<double>& values() const noexcept
        {
            return m_values;
        }

        [[nodiscard]]
        const std::vector<double>& times() const noexcept
        {
            return m_times;
        }

        [[nodiscard]]
        value_type front() const
        {
            return m_values.front();
        }

        [[nodiscard]]
        value_type back() const
        {
            return m_values.back();
        }

        [[nodiscard]]
        double startTime() const
        {
            return m_times.front();
        }

        [[nodiscard]]
        double endTime() const
        {
            return m_times.back();
        }

        [[nodiscard]]
        const_iterator begin() const 
        {
            return m_values.begin();
        }

        [[nodiscard]]
        const_iterator end() const 
        {
            return m_values.end();
        }

    private:
        void Validate() const
        {
            assert(!m_values.empty() && "Path must not be empty");

            assert(m_values.size() == m_times.size() && "Path must have equal amount of times and values");

            for (std::size_t i = 1; i < m_times.size(); ++i)
            {
                assert(m_times[i] > m_times[i - 1] && "Path times must be strictly increasing");
            }
        }

    private:
        std::vector<value_type> m_values;
        std::vector<value_type> m_times;
    };
}