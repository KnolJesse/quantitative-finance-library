#include <gtest/gtest.h>

#include <cstddef>
#include <cmath>

#include "qf/Pricing/BlackScholesGeometricAsianPricer.hpp"
#include "qf/Pricing/BlackScholesPricer.hpp"
#include "qf/Products/OptionType.hpp"

class BlackScholesGeometricAsianPricerTest : public ::testing::Test
{
protected:
    static constexpr double spot = 100.0;
    static constexpr double strike = 100.0;
    static constexpr double riskFreeRate = 0.05;
    static constexpr double volatility = 0.20;
    static constexpr double maturity = 1.0;
    
    static constexpr std::size_t numberOfObservations = 12; 

    double n = static_cast<double>(numberOfObservations);

    const double effVolatility =
        volatility * std::sqrt(
            (n + 1.0) * (2.0 * n + 1.0)
            /
            (6.0 * n * n));

    const double effDrift =
        (riskFreeRate - 0.5 * volatility * volatility)
        *
        ((n + 1.0) / (2 * n))
        +
        0.5 * effVolatility * effVolatility; 

    const qf::BlackScholesGeometricAsianPricer callPricer{
        qf::OptionType::Call,
        spot,
        strike,
        riskFreeRate,
        volatility,
        maturity,
        numberOfObservations
    };

    const qf::BlackScholesGeometricAsianPricer putPricer{
        qf::OptionType::Put,
        spot,
        strike,
        riskFreeRate,
        volatility,
        maturity,
        numberOfObservations
    };

    const qf::BlackScholesGeometricAsianPricer singleObservationCallPricer{
        qf::OptionType::Call,
        spot,
        strike,
        riskFreeRate,
        volatility,
        maturity,
        std::size_t(1)
    };

    const qf::BlackScholesPricer europeanCallPricer{
        qf::OptionType::Call,
        spot,
        strike,
        riskFreeRate,
        volatility,
        maturity,
    };

    const qf::BlackScholesGeometricAsianPricer zeroVolatilityCallPricer{
        qf::OptionType::Call,
        spot,
        strike,
        riskFreeRate,
        0.0,
        maturity,
        numberOfObservations
    };

    const qf::BlackScholesGeometricAsianPricer zeroVolatilityPutPricer{
        qf::OptionType::Put,
        spot,
        strike,
        riskFreeRate,
        0.0,
        maturity,
        numberOfObservations
    };

};

TEST_F(BlackScholesGeometricAsianPricerTest, GeometricAsianCallPrice)
{
    constexpr double referencePrice = 5.9402;

    EXPECT_NEAR(callPricer.Price(), referencePrice, 1e-4);
}

TEST_F(BlackScholesGeometricAsianPricerTest, GeometricAsianPutPrice)
{
    constexpr double referencePrice = 3.6517;

    EXPECT_NEAR(putPricer.Price(), referencePrice, 1e-4);
}

TEST_F(BlackScholesGeometricAsianPricerTest, ZeroVolatilityGeometricAsianCallPrice)
{
    constexpr double referencePrice = 2.6115;

    EXPECT_NEAR(zeroVolatilityCallPricer.Price(), referencePrice, 1e-4);
}

TEST_F(BlackScholesGeometricAsianPricerTest, ZeroVolatilityGeometricAsianPutPrice)
{
    constexpr double referencePrice = 0.0;

    EXPECT_NEAR(zeroVolatilityPutPricer.Price(), referencePrice, 1e-4);
}

TEST_F(BlackScholesGeometricAsianPricerTest, OneObservationReducesToEuropean)
{
    EXPECT_NEAR(singleObservationCallPricer.Price(), europeanCallPricer.Price(), 1e-4);
}

TEST_F(BlackScholesGeometricAsianPricerTest, GeometricAsianPutCallPriceParity)
{
    const double lhs = callPricer.Price() - putPricer.Price();

    const double rhs =
        std::exp(-riskFreeRate * maturity)
        *
        (spot * std::exp(effDrift * maturity) - strike); 

    EXPECT_NEAR(lhs, rhs, 1e-12);
}