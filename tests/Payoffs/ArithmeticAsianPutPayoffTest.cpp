#include <gtest/gtest.h>

#include "qf/PathGeneration/Path.hpp"
#include "qf/Payoffs/ArithmeticAsianPutPayoff.hpp"

//TEST(ArithmeticAsianPutPayoff, OutOfTheMoney)
//{
//    qf::ArithmeticAsianPutPayoff payoff(95.0);
//
//    qf::Path path({0.0, 1.0, 2.0}, {90.0, 100.0, 110.0});
//
//    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 0.0);
//}
//
//TEST(ArithmeticAsianPutPayoff, AtTheMoney)
//{
//    qf::ArithmeticAsianPutPayoff payoff(100.0);
//
//    qf::Path path({0.0, 1.0, 2.0}, { 90.0, 100.0, 110.0 });
//
//    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 0.0);
//}
//
//TEST(ArithmeticAsianPutPayoff, InTheMoney)
//{
//    qf::ArithmeticAsianPutPayoff payoff(105.0);
//
//    qf::Path path({0.0, 1.0, 2.0}, { 90.0, 100.0, 110.0 });
//
//    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 5.0);
//}

TEST(ArithmeticAsianPutPayoff, OutOfTheMoney)
{
    std::vector<double> times = { 0.0, 1.0, 2.0 };
    std::vector<double> values = { 90.0, 100.0, 110.0 };

    qf::ObservationSchedule schedule(times);

    qf::ArithmeticAsianPutPayoff payoff(95.0, schedule);

    qf::Path path(times, values);

    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 0.0);
}

TEST(ArithmeticAsianPutPayoff, AtTheMoney)
{
    std::vector<double> times = { 0.0, 1.0, 2.0 };
    std::vector<double> values = { 90.0, 100.0, 110.0 };

    qf::ObservationSchedule schedule(times);

    qf::ArithmeticAsianPutPayoff payoff(100.0, schedule);

    qf::Path path(times, values);

    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 0.0);
}

TEST(ArithmeticAsianPutPayoff, InTheMoney)
{
    std::vector<double> times = { 0.0, 1.0, 2.0 };
    std::vector<double> values = { 90.0, 100.0, 110.0 };

    qf::ObservationSchedule schedule(times);

    qf::ArithmeticAsianPutPayoff payoff(105.0, schedule);

    qf::Path path(times, values);

    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 5.0);
}

TEST(ArithmeticAsianPutPayoff, AveragesOverObservationSchedule1)
{
    std::vector<double> times = { 0.00, 0.25, 0.50, 0.75, 1.00 };
    std::vector<double> values = { 50, 100, 50, 100, 50 };

    qf::ObservationSchedule schedule({ 0.25, 0.75 });

    qf::ArithmeticAsianPutPayoff payoff(95.0, schedule);

    qf::Path path(times, values);

    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 0.0);
}

TEST(ArithmeticAsianPutPayoff, AveragesOverObservationSchedule2)
{
    std::vector<double> times = { 0.00, 0.25, 0.50, 0.75, 1.00 };
    std::vector<double> values = { 50, 100, 50, 100, 50 };

    qf::ObservationSchedule schedule({ 0.00, 0.50, 1.00 });

    qf::ArithmeticAsianPutPayoff payoff(95.0, schedule);

    qf::Path path(times, values);

    EXPECT_DOUBLE_EQ(payoff.Evaluate(path), 45.0);
}