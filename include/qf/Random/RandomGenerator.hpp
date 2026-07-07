#pragma once 

#include <cstdint>
#include <random>

namespace qf
{
    class RandomGenerator
    {
    public:
        RandomGenerator() = default;

        explicit RandomGenerator(std::uint64_t seed) noexcept
            : m_engine(seed)
        {
        }

    public:
        void Seed(std::uint64_t seed) noexcept
        {
            m_engine.seed(seed);
        }

    public:
        [[nodiscard]]
        double Uniform()
        {
            return std::uniform_real_distribution<double>{0.0, 1.0}(m_engine);
        }

        [[nodiscard]]
        double Normal()
        {
            return m_normalDistribution(m_engine);
        }

    private:
        std::mt19937_64 m_engine;
        std::normal_distribution<double> m_normalDistribution{ 0.0, 1.0 };
    };
}