#include <cstdint>
#include <random>

namespace qf
{
    class RandomGenerator
    {
    public:
        RandomGenerator() = default;

    public:
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
            return std::normal_distribution<double>{0.0, 1.0}(m_engine);
        }

    private:
        std::mt19937_64 m_engine;
    };

}