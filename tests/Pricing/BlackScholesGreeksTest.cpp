#include <gtest/gtest.h>

#include "qf/Pricing/BlackScholesPricer.hpp"
#include "qf/Products/OptionType.hpp"

class BlackScholesGreeksTest : public ::testing::Test
{
protected:
    static constexpr double spot = 100.0;
    static constexpr double strike = 100.0;
    static constexpr double riskFreeRate = 0.05;
    static constexpr double volatility = 0.20;
    static constexpr double maturity = 1.0;

    const qf::BlackScholesPricer callPricer{
        qf::OptionType::Call, 
        spot,
        strike,
        riskFreeRate,
        volatility,
        maturity
    };

    const qf::BlackScholesPricer putPricer{
        qf::OptionType::Put,
        spot,
        strike,
        riskFreeRate,
        volatility,
        maturity
    };
};

// Vega is quoted per unit volatility (not per 1%)
// Theta is quoted per year (not per day) 
// Rho is quoted per unit interest rate (not per 1%)

TEST_F(BlackScholesGreeksTest, CallDelta)
{
    constexpr double referenceDelta = 0.6368;

    EXPECT_NEAR(callPricer.Delta(), referenceDelta, 1e-4);
}

TEST_F(BlackScholesGreeksTest, CallGamma)
{
    constexpr double referenceGamma = 0.018762;

    EXPECT_NEAR(callPricer.Gamma(), referenceGamma, 1e-6);
}

TEST_F(BlackScholesGreeksTest, CallVega)
{
    constexpr double referenceVega = 37.5240;

    EXPECT_NEAR(callPricer.Vega(), referenceVega, 1e-3);
}

TEST_F(BlackScholesGreeksTest, CallTheta)
{
    constexpr double referenceTheta = -6.4140;

    EXPECT_NEAR(callPricer.Theta(), referenceTheta, 1e-3);
}

TEST_F(BlackScholesGreeksTest, CallRho)
{
    constexpr double referenceRho = 53.2325;

    EXPECT_NEAR(callPricer.Rho(), referenceRho, 1e-3);
}

TEST_F(BlackScholesGreeksTest, PutCallPriceParity)
{
    const double lhs = callPricer.Price() - putPricer.Price();

    const double rhs = spot - strike * std::exp(-riskFreeRate * maturity);

    EXPECT_NEAR(lhs, rhs, 1e-12); 
}

TEST_F(BlackScholesGreeksTest, PutCallDeltaParity)
{
    EXPECT_NEAR(callPricer.Delta() - putPricer.Delta(), 1.0, 1e-12);
}

TEST_F(BlackScholesGreeksTest, PutCallGammaParity)
{
    EXPECT_NEAR(callPricer.Gamma(), putPricer.Gamma(), 1e-12);
}

TEST_F(BlackScholesGreeksTest, PutCallVegaParity)
{
    EXPECT_NEAR(callPricer.Vega(), putPricer.Vega(), 1e-12);
}

TEST_F(BlackScholesGreeksTest, PutCallThetaParity)
{
    const double lhs = callPricer.Theta() - putPricer.Theta();

    const double rhs = -riskFreeRate * strike * std::exp(-riskFreeRate * maturity); 

    EXPECT_NEAR(lhs, rhs, 1e-12);
}

TEST_F(BlackScholesGreeksTest, PutCallRhoParity)
{
    const double lhs = callPricer.Rho() - putPricer.Rho();

    const double rhs = strike * maturity * std::exp(-riskFreeRate * maturity);

    EXPECT_NEAR(lhs, rhs, 1e-12);
}


