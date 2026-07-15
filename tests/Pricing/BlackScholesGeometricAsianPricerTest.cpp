#include <gtest/gtest.h>

#include <cstddef>
#include <cmath>

#include "qf/Market/MarketScenario.hpp"
#include "qf/Pricing/Analytical/BlackScholesGeometricAsianPricer.hpp"
#include "qf/Pricing/Analytical/BlackScholesEuropeanPricer.hpp"
#include "qf/Products/OptionType.hpp"
#include "qf/Time/ObservationSchedule.hpp"

//class BlackScholesGeometricAsianPricerTest : public ::testing::Test
//{
//protected:
//    static constexpr double strike = 100.0;
//
//    static constexpr double spot = 100.0;
//    static constexpr double riskFreeRate = 0.05;
//    static constexpr double volatility = 0.20;
//    static constexpr double maturity = 1.0;
//
//    static constexpr std::size_t numberOfObservations = 12; 
//
//    double n = static_cast<double>(numberOfObservations);
//
    //const double effVolatility =
    //    volatility * std::sqrt(
    //        (n + 1.0) * (2.0 * n + 1.0)
    //        /
    //        (6.0 * n * n));

    //const double effDrift =
    //    (riskFreeRate - 0.5 * volatility * volatility)
    //    *
    //    ((n + 1.0) / (2 * n))
    //    +
    //    0.5 * effVolatility * effVolatility; 
//
//    const qf::BlackScholesGeometricAsianPricer callPricer{
//        qf::OptionType::Call,
//        spot,
//        strike,
//        riskFreeRate,
//        volatility,
//        maturity,
//        numberOfObservations
//    };
//
//    const qf::BlackScholesGeometricAsianPricer putPricer{
//        qf::OptionType::Put,
//        spot,
//        strike,
//        riskFreeRate,
//        volatility,
//        maturity,
//        numberOfObservations
//    };
//
//    const qf::BlackScholesGeometricAsianPricer singleObservationCallPricer{
//        qf::OptionType::Call,
//        spot,
//        strike,
//        riskFreeRate,
//        volatility,
//        maturity,
//        std::size_t(1)
//    };
//
//    const qf::BlackScholesEuropeanPricer europeanCallPricer{
//        qf::OptionType::Call,
//        spot,
//        strike,
//        riskFreeRate,
//        volatility,
//        maturity,
//    };
//
//    const qf::BlackScholesGeometricAsianPricer zeroVolatilityCallPricer{
//        qf::OptionType::Call,
//        spot,
//        strike,
//        riskFreeRate,
//        0.0,
//        maturity,
//        numberOfObservations
//    };
//
//    const qf::BlackScholesGeometricAsianPricer zeroVolatilityPutPricer{
//        qf::OptionType::Put,
//        spot,
//        strike,
//        riskFreeRate,
//        0.0,
//        maturity,
//        numberOfObservations
//    };
//
//};
class BlackScholesGeometricAsianPricer : public ::testing::Test
{
protected:
    static constexpr double strike = 100.0;
    static constexpr double timeToMaturity = 1.0;

    static constexpr double spot = 100.0;
    static constexpr double riskFreeRate = 0.05;
    static constexpr double volatility = 0.20;

    static constexpr std::size_t numberOfObservations = static_cast<std::size_t>(12);

    qf::MarketScenario scenario{
        spot, 
        volatility, 
        riskFreeRate
    }; 

    qf::MarketScenario zeroVolScenario{
        spot,
        0.0,
        riskFreeRate
    };

    qf::ObservationSchedule schedule{
        1.0/12.0,
        2.0/12.0,
        3.0/12.0,
        4.0/12.0,
        5.0/12.0,
        6.0/12.0,
        7.0/12.0,
        8.0/12.0,
        9.0/12.0,
        10.0/12.0,
        11.0/12.0,
        1.0
    }; // MVP :( 
};

TEST_F(BlackScholesGeometricAsianPricer, GeometricAsianCallPrice)
{
    double referencePrice = 5.9402;

    qf::BlackScholesGeometricAsianPricer callPricer(qf::OptionType::Call, strike, timeToMaturity, schedule);

    double pricerPrice = callPricer.Price(scenario).Value;

    EXPECT_NEAR(pricerPrice, referencePrice, 1e-4);
}

TEST_F(BlackScholesGeometricAsianPricer, GeometricAsianPutPrice)
{
    constexpr double referencePrice = 3.6517;

    qf::BlackScholesGeometricAsianPricer callPricer(qf::OptionType::Put, strike, timeToMaturity, schedule);

    double pricerPrice = callPricer.Price(scenario).Value;

    EXPECT_NEAR(pricerPrice, referencePrice, 1e-4);
}

TEST_F(BlackScholesGeometricAsianPricer, ZeroVolatilityGeometricAsianCallPrice)
{
    constexpr double referencePrice = 2.6115;

    qf::BlackScholesGeometricAsianPricer callPricer(qf::OptionType::Call, strike, timeToMaturity, schedule);

    double pricerPrice = callPricer.Price(zeroVolScenario).Value;

    EXPECT_NEAR(pricerPrice, referencePrice, 1e-4);
}

TEST_F(BlackScholesGeometricAsianPricer, ZeroVolatilityGeometricAsianPutPrice)
{
    constexpr double referencePrice = 0.0;

    qf::BlackScholesGeometricAsianPricer putPricer(qf::OptionType::Put, strike, timeToMaturity, schedule);

    double pricerPrice = putPricer.Price(zeroVolScenario).Value;

    EXPECT_NEAR(pricerPrice, referencePrice, 1e-4);
}
TEST_F(BlackScholesGeometricAsianPricer, OneObservationReducesToEuropean)
{
    qf::ObservationSchedule singleSchedule{timeToMaturity};

    qf::BlackScholesGeometricAsianPricer asianCallPricer(qf::OptionType::Call, strike, timeToMaturity, singleSchedule);
    
    qf::BlackScholesEuropeanPricer europeanCallPricer(qf::OptionType::Call, strike, timeToMaturity);

    double asianPrice = asianCallPricer.Price(scenario).Value;

    double europeanPrice = europeanCallPricer.Price(scenario).Value;

    EXPECT_NEAR(asianPrice, europeanPrice, 1e-4);
}

TEST_F(BlackScholesGeometricAsianPricer, GeometricAsianPutCallPriceParity)
{
    qf::BlackScholesGeometricAsianPricer callPricer(qf::OptionType::Call, strike, timeToMaturity, schedule);
    qf::BlackScholesGeometricAsianPricer putPricer(qf::OptionType::Put, strike, timeToMaturity, schedule);

    const double effVolatility =
        volatility * std::sqrt(
            (numberOfObservations + 1.0) * (2.0 * numberOfObservations + 1.0)
            /
            (6.0 * numberOfObservations * numberOfObservations));

    const double effDrift =
        (riskFreeRate - 0.5 * volatility * volatility)
        *
        ((numberOfObservations + 1.0) / (2 * numberOfObservations))
        +
        0.5 * effVolatility * effVolatility;


    const double lhs = callPricer.Price(scenario).Value - putPricer.Price(scenario).Value;

    const double rhs =
        std::exp(-scenario.RiskFreeRate * timeToMaturity)
        *
        (spot * std::exp(effDrift * timeToMaturity) - strike); 

    EXPECT_NEAR(lhs, rhs, 1e-12);
}