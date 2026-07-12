#include <gtest/gtest.h>

#include <cmath>

#include "qf/PathGeneration/Path.hpp"
#include "qf/Payoffs/GeometricAsianCallPayoff.hpp"
#include "qf/Time/ObservationSchedule.hpp"

TEST(GeometricAsianCallPayoff, OutOfTheMoney)
{
    std::vector<double> times = { 0.00, 0.25, 0.50, 0.75 };
    std::vector<double> values = { 1.0, 2.0, 4.0, 8.0 };

    qf::ObservationSchedule schedule(times);

    qf::GeometricAsianCallPayoff payoff(3.0, schedule);

    qf::Path path(times, values);

    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 0.0);
}

TEST(GeometricAsianCallPayoff, AtTheMoney)
{
    std::vector<double> times = { 0.00, 0.25, 0.50, 0.75 };
    std::vector<double> values = { 1.0, 2.0, 4.0, 8.0 };

    double geometricMean = 2.0 * std::sqrt(2); 

    qf::ObservationSchedule schedule(times);

    qf::GeometricAsianCallPayoff payoff(geometricMean, schedule);

    qf::Path path(times, values);

    EXPECT_NEAR(payoff.Evaluate(path), 0.0, 1e-12);
}

TEST(GeometricAsianCallPayoff, InTheMoney)
{
    std::vector<double> times = { 0.00, 0.25, 0.50, 0.75 };
    std::vector<double> values = { 1.0, 2.0, 4.0, 8.0 };

    double geometricMean = 2.0 * std::sqrt(2);

    qf::ObservationSchedule schedule(times);

    qf::GeometricAsianCallPayoff payoff(2.0, schedule);

    qf::Path path(times, values);

    EXPECT_NEAR(payoff.Evaluate(path), geometricMean - 2.0, 1e-12);
}

TEST(GeometricAsianCallPayoff, UsesGeometricNotArithmeticAverage)
{
    std::vector<double> times = { 0.00, 0.25, 0.50, 0.75 };
    std::vector<double> values = { 1.0, 2.0, 4.0, 8.0 };
    
    qf::ObservationSchedule schedule(times);

    qf::Path path(times, values);
    
    qf::GeometricAsianCallPayoff payoff(3.0, schedule);

    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 0.0);
}