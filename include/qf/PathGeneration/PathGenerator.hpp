#pragma once

#include "qf/PathGeneration/Path.hpp"
#include "qf/PathGeneration/SimulationGrid.hpp"
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
            RandomGenerator& randomGenerator) noexcept
            : m_model(model)
            , m_randomGenerator(randomGenerator)
        {
        }

    public:
        [[nodiscard]]
        Path Generate(double spot, double timeStep, std::size_t numberOfSteps) const 
        {
            std::vector<double> values;
            values.reserve(numberOfSteps + 1);

            std::vector<double> times; 
            times.reserve(numberOfSteps + 1); 

            double currentValue = spot;
            values.push_back(currentValue);

            double currentTime = 0.0; 
            times.push_back(currentTime); 

            for (std::size_t step = 0; step < numberOfSteps; ++step)
            {
                currentValue = m_model.Evolve(currentValue, timeStep, m_randomGenerator);

                currentTime += timeStep; 

                values.push_back(currentValue);
                times.push_back(currentTime);
            }

            //return Path(std::move(values), std::move(times));
            return Path(times, values); 
        }

        [[nodiscard]]
        Path Generate(double spot, SimulationGrid simulationGrid) 
        {
            return Generate(spot, simulationGrid.TimeStep(), simulationGrid.NumberOfSteps()); 
        }

    private:
        const Model& m_model;
        RandomGenerator& m_randomGenerator;
    };
}