#pragma once

#include <cstddef>
#include <utility>
#include <vector>

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

        explicit Path(std::vector<double> values) noexcept
            : m_values(std::move(values))
        {
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
        const std::vector<double>& values() const noexcept
        {
            return m_values;
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
        const_iterator begin() const noexcept
        {
            return m_values.begin();
        }

        [[nodiscard]]
        const_iterator end() const noexcept
        {
            return m_values.end();
        }

    private:
        std::vector<value_type> m_values;
    };
}