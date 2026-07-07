#include "qf/Models/GeometricBrownianMotion.hpp"

#include <cmath>
#include <cassert>

namespace qf
{
    double GeometricBrownianMotion::Evolve(double currentPrice, double timeStep, RandomGenerator& randomGenerator) const
    {
        assert(currentPrice > 0 && "currentPrice must be greater than zero."); 
        assert(timeStep > 0 && "timeStep must be greater than zero."); 
        assert(m_volatility >= 0 && "volatility must be non-negative."); 

        const double standardNormal = randomGenerator.Normal();

        const double driftTerm = (m_drift - 0.5 * m_volatility * m_volatility) * timeStep;

        const double diffusionTerm = m_volatility * std::sqrt(timeStep) * standardNormal;

        return currentPrice * std::exp(driftTerm + diffusionTerm);
    }
}