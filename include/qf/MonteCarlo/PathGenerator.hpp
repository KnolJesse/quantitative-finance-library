#pragma once

#include "qf/MonteCarlo/Path.hpp"
#include "qf/Random/RandomGenerator.hpp"

#include <cstddef>
#include <vector>
#include <utility>

namespace qf
{
    template<typename Model>
    class PathGenerator
    {
    public:
        PathGenerator(
            const Model& model, 
            RandomGenerator& randomGenerator,
            double initialValue,
            double timeStep,
            std::size_t numberOfSteps) noexcept
            : m_model(model)
            , m_randomGenerator(randomGenerator)
            , m_initialValue(initialValue)
            , m_timeStep(timeStep)
            , m_numberOfSteps(numberOfSteps)
        {
        }

    public:
        [[nodiscard]]
        Path Generate()
        {
            std::vector<double> values;
            values.reserve(m_numberOfSteps + 1);

            double currentValue = m_initialValue;

            values.push_back(currentValue);

            for (std::size_t step = 0; step < m_numberOfSteps; ++step)
            {
                currentValue = m_model.Evolve(currentValue, m_timeStep, m_randomGenerator);

                values.push_back(currentValue);
            }

            return Path(std::move(values));
        }

    private:
        const Model& m_model;
        RandomGenerator& m_randomGenerator;

        double m_initialValue = 0.0;
        double m_timeStep = 0.0;
        std::size_t m_numberOfSteps = 0;
    };
}