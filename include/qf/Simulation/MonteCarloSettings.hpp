#pragma once 

#include <cstddef> 

namespace qf
{
    struct MonteCarloSettings
    {
        double RequestedTimeStep = 1.0 / 252.0;
        std::size_t NumberOfSimulations = 100'000;

        bool UseAntitheticVariates = false;
        bool UseControlVariates = false; 
    };
}