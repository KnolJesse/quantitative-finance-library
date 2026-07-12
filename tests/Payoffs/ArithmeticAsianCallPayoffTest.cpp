#include <gtest/gtest.h>

#include "qf/PathGeneration/Path.hpp"
#include "qf/Payoffs/ArithmeticAsianCallPayoff.hpp"
#include "qf/Time/ObservationSchedule.hpp"

TEST(ArithmeticAsianCallPayoff, OutOfTheMoney)
{
    std::vector<double> times = { 0.0, 1.0, 2.0 }; 
    std::vector<double> values = { 90.0, 100.0, 110.0 }; 

    qf::ObservationSchedule schedule(times); 

    qf::ArithmeticAsianCallPayoff payoff(105.0, schedule);

    qf::Path path(times, values);

    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 0.0);
}

TEST(ArithmeticAsianCallPayoff, AtTheMoney)
{
    std::vector<double> times = { 0.0, 1.0, 2.0 };
    std::vector<double> values = { 90.0, 100.0, 110.0 };

    qf::ObservationSchedule schedule(times);

    qf::ArithmeticAsianCallPayoff payoff(100.0, schedule);

    qf::Path path(times, values);

    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 0.0);
}

TEST(ArithmeticAsianCallPayoff, InTheMoney)
{
    std::vector<double> times = { 0.0, 1.0, 2.0 };
    std::vector<double> values = { 90.0, 100.0, 110.0 };

    qf::ObservationSchedule schedule(times);

    qf::ArithmeticAsianCallPayoff payoff(95.0, schedule);

    qf::Path path(times, values);

    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 5.0);
}

TEST(ArithmeticAsianCallPayoff, AveragesOverObservationSchedule1)
{
    std::vector<double> times = {0.00, 0.25, 0.50, 0.75, 1.00};
    std::vector<double> values = {50, 100, 50, 100, 50};

    qf::ObservationSchedule schedule({0.25, 0.75});

    qf::ArithmeticAsianCallPayoff payoff(95.0, schedule);

    qf::Path path(times, values);

    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 5.0);
}

TEST(ArithmeticAsianCallPayoff, AveragesOverObservationSchedule2)
{
    std::vector<double> times = { 0.00, 0.25, 0.50, 0.75, 1.00 };
    std::vector<double> values = { 50, 100, 50, 100, 50 };

    qf::ObservationSchedule schedule({ 0.00, 0.50, 1.00 });

    qf::ArithmeticAsianCallPayoff payoff(95.0, schedule);

    qf::Path path(times, values);

    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 0.0);
}