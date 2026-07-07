#include <gtest/gtest.h>

#include "qf/Pricing/BlackScholesPricer.hpp"

class BlackScholesGreeksTest : public ::testing::Test
{
protected:
    static constexpr double Spot = 100.0;
    static constexpr double Strike = 100.0;
    static constexpr double RiskFreeRate = 0.05;
    static constexpr double Volatility = 0.20;
    static constexpr double Maturity = 1.0;

    qf::BlackScholesPricer Pricer{
        Spot,
        Strike,
        RiskFreeRate,
        Volatility,
        Maturity
    };
};

// Vega is quoted per unit volatility (not per 1%)
// Theta is quoted per year (not per day) 
// Rho is quoted per unit interest rate (not per 1%)

TEST_F(BlackScholesGreeksTest, Delta)
{
    constexpr double ReferenceDelta = 0.6368;

    EXPECT_NEAR(Pricer.Delta(), ReferenceDelta, 1e-4);
}

TEST_F(BlackScholesGreeksTest, Gamma)
{
    constexpr double ReferenceGamma = 0.018762;

    EXPECT_NEAR( Pricer.Gamma(), ReferenceGamma, 1e-6);
}

TEST_F(BlackScholesGreeksTest, Vega)
{
    constexpr double ReferenceVega = 37.5240;

    EXPECT_NEAR( Pricer.Vega(), ReferenceVega, 1e-3);
}

TEST_F(BlackScholesGreeksTest, Theta)
{
    constexpr double ReferenceTheta = -6.4140;

    EXPECT_NEAR(Pricer.Theta(), ReferenceTheta, 1e-3);
}

TEST_F(BlackScholesGreeksTest, Rho)
{
    constexpr double ReferenceRho = 53.2325;

    EXPECT_NEAR(Pricer.Rho(), ReferenceRho, 1e-3);
}