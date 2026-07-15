#pragma once

#include "qf/Market/MarketScenario.hpp"
#include "qf/Products/OptionType.hpp"
#include "qf/Pricing/PricingResult.hpp"
#include "qf/Time/ObservationSchedule.hpp"

#include <cassert>
#include <cstddef>
#include <cmath>
#include <span>
#include <utility>


namespace qf
{
    class BlackScholesGeometricAsianPricer
    {
    public:
        BlackScholesGeometricAsianPricer(
            OptionType optionType,
            double strike,
            double maturity,
            ObservationSchedule observationSchedule) 
            : m_optionType(optionType)
            , m_strike(strike)
            , m_maturity(maturity)
            , m_schedule(std::move(observationSchedule))
        {
            assert(m_strike > 0.0 && "strike must be greater than zero");
            assert(m_maturity > 0.0 && "maturity must be greater than zero");
            assert(!m_schedule.empty() && "observationschedule must not be empty");
        }

    public:
        [[nodiscard]]
        PricingResult Price(const MarketScenario& scenario) const
        {
            switch (m_optionType)
            {
            case OptionType::Call:
                return CallPrice(scenario);

            case OptionType::Put:
                return PutPrice(scenario);
            }

            assert(false && "Option type not supported");

            return PricingResult{ 0.0, 0.0 };
        }

    private:
        struct BlackScholesTerms
        {
            double D1 = 0.0;
            double D2 = 0.0;
        };

    private:
        [[nodiscard]]
        PricingResult CallPrice(const MarketScenario& scenario) const
        {
            PricingResult result; 

            result.StandardError = 0.0; 

            if (scenario.Volatility == 0.0)
            {
                double geometricAverage = scenario.Spot * std::exp(scenario.RiskFreeRate * m_schedule.AverageObservationTime());

                result.Value = std::exp(-scenario.RiskFreeRate * m_maturity) * std::max(geometricAverage - m_strike, 0.0);
                
                return result; 
            }

            double effectiveVariance = EffectiveVariance(scenario);
            double effectiveDrift = EffectiveDrift(scenario, effectiveVariance); 

            BlackScholesTerms terms = ComputeTerms(scenario, effectiveVariance, effectiveDrift); 

            result.Value = 
                std::exp(-scenario.RiskFreeRate * m_maturity)
                *
                (scenario.Spot * std::exp(effectiveDrift * m_maturity) * NormalCDF(terms.D1)
                    -
                    m_strike * NormalCDF(terms.D2));
            
            return result; 
        }

        [[nodiscard]]
        PricingResult PutPrice(const MarketScenario& scenario) const
        {
            PricingResult result;

            result.StandardError = 0.0;

            if (scenario.Volatility == 0.0)
            {
                double geometricAverage = scenario.Spot * std::exp(scenario.RiskFreeRate * m_schedule.AverageObservationTime());

                result.Value = std::exp(-scenario.RiskFreeRate * m_maturity) * std::max(m_strike - geometricAverage, 0.0);
               
                return result; 
            }

            double effectiveVariance = EffectiveVariance(scenario);
            double effectiveDrift = EffectiveDrift(scenario, effectiveVariance);

            BlackScholesTerms terms = ComputeTerms(scenario, effectiveVariance, effectiveDrift);

            result.Value = 
                std::exp(-scenario.RiskFreeRate * m_maturity)
                *
                (m_strike * NormalCDF(-terms.D2)
                    -
                    scenario.Spot * std::exp(effectiveDrift * m_maturity) * NormalCDF(-terms.D1));
            
            return result; 
                
        }

        [[nodiscard]]
        static double NormalCDF(double x) noexcept
        {
            return 0.5 * std::erfc(-x / std::sqrt(2.0));
        }

        [[nodiscard]]
        double EffectiveVariance(const MarketScenario& scenario) const noexcept 
        {
            std::span<const double> times = m_schedule.ObservationTimes();

            std::size_t numberOfObservations = times.size();

            double normalization =
                scenario.Volatility * scenario.Volatility
                /
                static_cast<double>(numberOfObservations * numberOfObservations);

            double covarianceSum    = 0.0;
            double previousTimesSum = 0.0;

            for (std::size_t i = 0; i < numberOfObservations; ++i)
            {
                covarianceSum += previousTimesSum + times[i] * (numberOfObservations - i);

                previousTimesSum += times[i];
            }

            return normalization * covarianceSum; 
        }

        [[nodiscard]]
        double EffectiveDrift(
            const MarketScenario& scenario,
            double effectiveVariance) const noexcept
        {
            double averageObservationTime = m_schedule.AverageObservationTime(); 

            return
                (scenario.RiskFreeRate - 0.5 * scenario.Volatility * scenario.Volatility)
                *
                (averageObservationTime / m_maturity)
                +
                (0.5 * effectiveVariance); 
        }

        [[nodiscard]]
        BlackScholesTerms ComputeTerms(
            const MarketScenario& scenario,
            double effectiveVariance, 
            double effectiveDrift) const noexcept
        {
            BlackScholesTerms terms;

            double effectiveVolatility = std::sqrt(effectiveVariance); 

            double sigmaSqrtT = effectiveVolatility * std::sqrt(m_maturity);

            double term1 = std::log(scenario.Spot / m_strike);
            double term2 = (effectiveDrift + 0.5 * effectiveVariance) * m_maturity;

            terms.D1 = (term1 + term2) / sigmaSqrtT; 
            terms.D2 = terms.D1 - sigmaSqrtT; 
           
            return terms;
        }

    private:
        OptionType m_optionType = OptionType::Call;
        double m_strike = 0.0;
        double m_maturity = 0.0; 
        ObservationSchedule m_schedule; 
    };
}