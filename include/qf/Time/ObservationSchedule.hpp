#pragma once

#include <cassert>
#include <cstddef>
#include <span>
#include <vector>
#include <numeric>

namespace qf
{
    class ObservationSchedule
    {
    public:
        ObservationSchedule() = default;

        explicit ObservationSchedule(std::vector<double> observationTimes) noexcept
            : m_observationTimes(std::move(observationTimes))
        {
            assert(!m_observationTimes.empty() && "observation schedule must contain at least one observation");

            assert(m_observationTimes.front() >= 0.0 && "observation times must be non-negative");

            for (std::size_t i = 1; i < m_observationTimes.size(); ++i)
            {
                assert(m_observationTimes[i] > m_observationTimes[i - 1] && "observation times must be strictly increasing");
            }
        }

        ObservationSchedule(double endTime, std::size_t numberOfObservations)
        {
            assert(endTime > 0.0 && "endTime must be greater than zero");

            assert(numberOfObservations > 0 && "numberOfObservations must be greater than zero");

            m_observationTimes.reserve(numberOfObservations);

            const double timeStep = endTime / static_cast<double>(numberOfObservations);

            for (std::size_t i = 1; i <= numberOfObservations; ++i)
            {
                m_observationTimes.push_back(static_cast<double>(i) * timeStep);
            }
        }

        ObservationSchedule(std::initializer_list<double> observationTimes) 
            : m_observationTimes(observationTimes) 
        { 
            assert(!m_observationTimes.empty() && "observation schedule must contain at least one observation");

            assert(m_observationTimes.front() >= 0.0 && "observation times must be non-negative");

            for (std::size_t i = 1; i < m_observationTimes.size(); ++i)
            {
                assert(m_observationTimes[i] > m_observationTimes[i - 1] && "observation times must be strictly increasing");
            }
        }

    public:
        [[nodiscard]]
        std::size_t size() const
        {
            return m_observationTimes.size();
        }

        [[nodiscard]]
        bool empty() const
        {
            return m_observationTimes.empty();
        }

        [[nodiscard]]
        double at(std::size_t index) const
        {
            return m_observationTimes.at(index);
        }

        [[nodiscard]]
        double front() const
        {
            return m_observationTimes.front();
        }

        [[nodiscard]]
        double back() const
        {
            return m_observationTimes.back();
        }

        [[nodiscard]]
        auto begin() const
        {
            return m_observationTimes.begin();
        }

        [[nodiscard]]
        auto end() const
        {
            return m_observationTimes.end();
        }

    public: 
        [[nodiscard]]
        std::span<const double> ObservationTimes() const
        {
            return m_observationTimes;
        }

        [[nodiscard]]
        double AverageObservationTime() const
        {
            double timesSum = std::accumulate(m_observationTimes.begin(), m_observationTimes.end(), 0.0);
            double numberOfObservations = static_cast<double>(m_observationTimes.size()); 
            
            return timesSum / numberOfObservations; 
        }

        [[nodiscard]]
        ObservationSchedule FormUniformSchedule(double endTime, std::size_t numberOfObservations)
        {
            assert(endTime > 0.0 && "endTime must be greater than zero");

            assert(numberOfObservations > 0 && "numberOfObservations must be greater than zero");

            m_observationTimes.reserve(numberOfObservations);

            const double timeStep = endTime / static_cast<double>(numberOfObservations);

            for (std::size_t i = 1; i <= numberOfObservations; ++i)
            {
                m_observationTimes.push_back(static_cast<double>(i) * timeStep);
            }
        }

        [[nodiscard]]
        ObservationSchedule AdvanceBy(double dt) const
        {
            assert(dt >= 0.0 && "dt must be non-negative");

            std::vector<double> newSchedule;

            newSchedule.reserve(m_observationTimes.size()); 

            for (double time : m_observationTimes)
            {
                double newTime = time - dt;

                if (newTime >= 0.0) 
                {
                    newSchedule.push_back(newTime);
                }
            }

            return ObservationSchedule(newSchedule); 
        }


    private:
        std::vector<double> m_observationTimes;
    };
}