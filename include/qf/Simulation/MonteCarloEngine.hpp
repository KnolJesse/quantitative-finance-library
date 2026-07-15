#pragma once 

#include <cmath>
#include <cstddef>
#include <iostream>

#include "qf/PathGeneration/PathGenerator.hpp"
#include "qf/PathGeneration/SimulationGrid.hpp"
#include "qf/Payoffs/Payoff.hpp"
#include "qf/Random/RandomGenerator.hpp"
#include "qf/Simulation/SimulationResult.hpp"
#include "qf/Statistics/RunningStats.hpp"

namespace qf
{
	template<typename Model>
	class MonteCarloEngine
	{
	public:
		[[nodiscard]]
		SimulationResult Simulate(
			const Payoff& payoff,
			double spot,
			const Model& model,
			RandomGenerator& randomGenerator,
			const SimulationGrid& simulationGrid,
			const std::size_t numberOfSimulations) const
		{
			RunningStats statistics;

			PathGenerator<Model> pathGenerator(model, randomGenerator);

			for (std::size_t s = 0; s < numberOfSimulations; ++s)
			{
				const Path path = pathGenerator.Generate(spot, simulationGrid);

				const double payoffValue = payoff.Evaluate(path);

				statistics.Add(payoffValue);
			}

			SimulationResult result;

			result.Mean = statistics.Mean();
			result.StandardError = std::sqrt(statistics.Variance() / static_cast<double>(numberOfSimulations));

			return result;
		}
	};
}