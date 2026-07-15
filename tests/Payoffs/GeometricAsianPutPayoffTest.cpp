#include <gtest/gtest.h>

#include "qf/PathGeneration/Path.hpp"
#include "qf/Payoffs/GeometricAsianPutPayoff.hpp"
#include "qf/Time/ObservationSchedule.hpp"

TEST(GeometricAsianPutPayoff, OutOfTheMoney)
{
    std::vector<double> times = { 0.00, 0.25, 0.50, 0.75 };
    std::vector<double> values = { 1.0, 2.0, 4.0, 8.0 };

    qf::ObservationSchedule schedule(times);

    qf::GeometricAsianPutPayoff payoff(2.0, 0.75, schedule);

    qf::Path path(times, values);

    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 0.0);
}

TEST(GeometricAsianPutPayoff, AtTheMoney)
{
    std::vector<double> times = { 0.00, 0.25, 0.50, 0.75 };
    std::vector<double> values = { 1.0, 2.0, 4.0, 8.0 };

    double geometricMean = 2.0 * std::sqrt(2);

    qf::ObservationSchedule schedule(times);

    qf::GeometricAsianPutPayoff payoff(geometricMean, 0.75, schedule);

    qf::Path path(times, values);

    EXPECT_NEAR(payoff.Evaluate(path), 0.0, 1e-12);
}

TEST(GeometricAsianPutPayoff, InTheMoney)
{
    std::vector<double> times = { 0.00, 0.25, 0.50, 0.75 };
    std::vector<double> values = { 1.0, 2.0, 4.0, 8.0 };

    double geometricMean = 2.0 * std::sqrt(2);

    qf::ObservationSchedule schedule(times);

    qf::GeometricAsianPutPayoff payoff(3.0, 0.75, schedule);

    qf::Path path(times, values);

    EXPECT_NEAR(payoff.Evaluate(path), 3.0 - geometricMean, 1e-12);
}

//TEST(GeometricAsianPutPayoff, UsesGeometricNotArithmeticAverage)
//{
//    std::vector<double> times = { 0.00, 0.25, 0.50, 0.75 };
//    std::vector<double> values = { 1.0, 2.0, 4.0, 8.0 };
//
//    qf::ObservationSchedule schedule(times);
//
//    qf::Path path(times, values);
//
//    qf::GeometricAsianPutPayoff payoff(3.0, schedule);
//
//    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 0.0);
//}
//TEST(GeometricAsianPutPayoff, OutOfTheMoney)
//{
//    qf::GeometricAsianPutPayoff payoff(95.0);
//
//    qf::Path path({ 0.0, 1.0, 2.0 }, {100.0, 100.0, 100.0});
//
//    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 0.0);
//}
//
//TEST(GeometricAsianPutPayoff, AtTheMoney)
//{
//    qf::GeometricAsianPutPayoff payoff(100.0);
//
//    qf::Path path({ 0.0, 1.0, 2.0 }, {100.0, 100.0, 100.0});
//
//    EXPECT_NEAR(payoff.Evaluate(path), 0.0, 1e-12);
//}
//
//TEST(GeometricAsianPutPayoff, InTheMoney)
//{
//    qf::GeometricAsianPutPayoff payoff(105.0);
//
//    qf::Path path({ 0.0, 1.0, 2.0 }, {100.0, 100.0, 100.0});
//
//    EXPECT_NEAR(payoff.Evaluate(path), 5.0, 1e-12);
//}
//
//TEST(GeometricAsianPutPayoff, UsesGeometricNotArithmeticAverage)
//{
//    qf::GeometricAsianPutPayoff payoff(110.0);
//
//    qf::Path path({ 0.0, 1.0, 2.0 }, {50.0, 100.0, 200.0});
//
//    EXPECT_NEAR(payoff.Evaluate(path), 10.0, 1e-12);
//}

