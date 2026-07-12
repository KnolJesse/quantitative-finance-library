#pragma once 

#include <cassert>
#include <cstddef>

namespace qf
{
	class SimulationGrid
	{
	public:
		SimulationGrid(double timeStep, std::size_t numberOfSteps) noexcept
			: m_timeStep(timeStep)
			, m_numberOfSteps(numberOfSteps)
		{
			assert(timeStep > 0.0 && "timeStep must be greater than zero");
			assert(numberOfSteps > 0.0 && "numberOfSteps must be greater than zero");
		}

		SimulationGrid(double maturity, double requestedTimeStep) noexcept
		{
			assert(maturity > 0.0 && "maturity must be greater than zero");
			assert(maturity >= requestedTimeStep && "maturity must be greater than or equal to requestedTimeStep");
			assert(requestedTimeStep > 0.0 && "requestedTimeStep must be greater than zero");

			m_numberOfSteps = static_cast<std::size_t>(std::ceil(maturity / requestedTimeStep));
			m_timeStep = maturity / static_cast<double>(m_numberOfSteps);
		}

	public:
		[[nodiscard]]
		double TimeStep() const noexcept
		{
			return m_timeStep;
		}

		[[nodiscard]]
		std::size_t NumberOfSteps() const noexcept
		{
			return m_numberOfSteps;
		}

		[[nodiscard]]
		double TimeAt(std::size_t index) const noexcept
		{
			return static_cast<double>(index * m_timeStep); 
		}

	private:
		double m_timeStep;
		std::size_t m_numberOfSteps;
	};
}