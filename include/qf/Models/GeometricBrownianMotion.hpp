#pragma once 

#include "qf\Random\RandomGenerator.hpp"

namespace qf
{
	class GeometricBrownianMotion
	{
	public:
		GeometricBrownianMotion(double drift, double volatility) noexcept
			: m_drift(drift)
			, m_volatility(volatility)
		{
		}

	public:
		[[nodiscard]]
		double Evolve(double currentPrice, double timeStep, RandomGenerator& randomGenerator) const; 

	public: 
		[[nodiscard]]
		double Drift() const noexcept
		{
			return m_drift; 
		}

		[[nodiscard]]
		double Volatility() const noexcept
		{
			return m_volatility; 
		}

	private:
		double m_drift = 0.0; 
		double m_volatility = 0.0; 
	};
}