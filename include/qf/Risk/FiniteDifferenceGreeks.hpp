#pragma once 

#include <cmath>

#include "qf/Market/MarketScenario.hpp"
#include "qf/Pricing/PricingResult.hpp"
#include "qf/Risk/GreekResult.hpp"

namespace qf
{
	template<typename Pricer>
	class FiniteDifferenceGreeks
	{
	public:
		[[nodiscard]]
		GreekResult Delta(const Pricer& pricer, const MarketScenario& scenario, double bump) const
		{
			MarketScenario upScenario	= BumpScenario(scenario, ScenarioParameter::Spot, bump);
			MarketScenario downScenario = BumpScenario(scenario, ScenarioParameter::Spot, -bump);

			PricingResult upPrice	= Pricer.Price(upScenario); 
			PricingResult downPrice = Pricer.Price(downScenario); 

			GreekResult result; 

			result.Value =
				(upPrice.Value - downPrice.Value)
				/
				(2.0 * bump);

			result.StandardError = 
				std::sqrt(
					upPrice.StandardError * upPrice.StandardError 
					+
					downPrice.StandardError * downPrice.StandardError)
				/
				(2.0 * bump);

			return result
		}

		[[nodiscard]]
		GreekResult Vega(const Pricer& pricer, const MarketScenario& scenario, double bump) const
		{
			MarketScenario upScenario = BumpScenario(scenario, ScenarioParameter::Volatility, bump);
			MarketScenario downScenario = BumpScenario(scenario, ScenarioParameter::Volatility, -bump);

			PricingResult upPrice = Pricer.Price(upScenario);
			PricingResult downPrice = Pricer.Price(downScenario);

			GreekResult result;

			result.Value =
				(upPrice.Value - downPrice.Value)
				/
				(2.0 * bump);

			result.StandardError =
				std::sqrt(
					upPrice.StandardError * upPrice.StandardError
					+
					downPrice.StandardError * downPrice.StandardError)
				/
				(2.0 * bump);

			return result
		}

		[[nodiscard]]
		GreekResult Gamma(const Pricer& pricer, const MarketScenario& scenario, double bump) const
		{
			MarketScenario upScenario = BumpScenario(scenario, ScenarioParameter::Volatility, bump);
			MarketScenario downScenario = BumpScenario(scenario, ScenarioParameter::Volatility, -bump);

			PricingResult upPrice = Pricer.Price(upScenario);
			PricingResult price = Pricer.Price(scenario); 
			PricingResult downPrice = Pricer.Price(downScenario);

			GreekResult result;

			result.Value =
				(upPrice.Value - 2.0 * price.Value + downPrice.Value)
				/
				(bump * bump);

			result.StandardError =
				std::sqrt(
					upPrice.StandardError * upPrice.StandardError 
					+
					4.0 * price.StandardError * price.StandardError
					+
					downPrice.StandardError * downPrice.StandardError)
				/
				(bump * bump);

			return result
		}

		[[nodiscard]]
		GreekResult Rho(const Pricer& pricer, const MarketScenario& scenario, double bump) const
		{
			MarketScenario upScenario = BumpScenario(scenario, ScenarioParameter::RiskFreeRate, bump);
			MarketScenario downScenario = BumpScenario(scenario, ScenarioParameter::RiskFreeRate, -bump);

			PricingResult upPrice = Pricer.Price(upScenario);
			PricingResult downPrice = Pricer.Price(downScenario);

			GreekResult result;

			result.Value =
				(upPrice.Value - downPrice.Value)
				/
				(2.0 * bump);

			result.StandardError =
				std::sqrt(
					upPrice.StandardError * upPrice.StandardError
					+
					downPrice.StandardError * downPrice.StandardError)
				/
				(2.0 * bump);

			return result
		}

		[[nodiscard]]
		GreekResult Theta(const Pricer& pricer, const MarketScenario& scenario, double bump) const

	private:
		enum class ScenarioParameter
		{
			Spot,
			Volatility,
			RiskFreeRate,
		};

		[[nodiscard]]
		MarketScenario BumpScenario(MarketScenario scenario, ScenarioParameter parameter, double bump)
		{
			MarketScenario newScenario = scenario; 

			switch (parameter)
			{
			case ScenarioParameter::Spot:
				out.Spot += bump;

			case ScenarioParameter::Volatility:
				out.Volatility += bump;

			case ScenarioParameter::RiskFreeRate:
				out.RiskFreeRate += bump; 
			}

			return newScenario
		}
		
	
	private:
		const Pricer& m_pricer; 
	};
}

///**
//         * @brief Estimates Delta using a central finite difference.
//         *
//         * Two Monte Carlo simulations are performed using bumped spot
//         * prices. Common Random Numbers are used to reduce the variance
//         * of the finite difference estimator.
//         *
//         * @param bump Absolute perturbation applied to the spot price.
//         *
//         * @return Estimated Delta.
//         */
//[[nodiscard]]
//double Delta(double bump) const
//{
//    MarketScenario upScenario, downScenario;
//    upScenario = downScenario = m_marketScenario;
//
//    upScenario.Spot += bump;
//    downScenario.Spot -= bump;
//
//    PricingResult up = PriceScenario(upScenario);
//    PricingResult down = PriceScenario(downScenario);
//
//    return
//        (up.Value - down.Value)
//        /
//        (2.0 * bump);
//}
//
///**
// * @brief Estimates Gamma using a central finite difference.
// *
// * Three Monte Carlo simulations are performed using bumped  and current spot
// * prices. Common Random Numbers are used to reduce the variance
// * of the finite difference estimator.
// *
// * @param bump Absolute perturbation applied to the spot price.
// *
// * @return Estimated Gamma.
// */
//[[nodiscard]]
//double Gamma(double bump) const
//{
//    MarketScenario upScenario, downScenario;
//    upScenario = downScenario = m_marketScenario;
//
//    upScenario.Volatility += bump;
//    downScenario.Volatility -= bump;
//
//    PricingResult up = PriceScenario(upScenario);
//    PricingResult middle = Price();
//    PricingResult down = PriceScenario(downScenario);
//
//    return
//        (up.Value - 2.0 * middle.Value + down.Value)
//        /
//        (bump * bump);
//}
//
///**
// * @brief Estimates Vega using a central finite difference.
// *
// * Two Monte Carlo simulations are performed using bumped model
// * volatilities. Common Random Numbers (CRN) are used to reduce the
// * variance of the finite difference estimator.
// *
// * @param bump Absolute perturbation applied to the model volatility.
// *
// * @return Estimated Vega.
// */
//[[nodiscard]]
//double Vega(double bump) const
//{
//    MarketScenario upScenario, downScenario;
//    upScenario = downScenario = m_marketScenario;
//
//    upScenario.Volatility += bump;
//    downScenario.Volatility -= bump;
//
//    PricingResult up = PriceScenario(upScenario);
//    PricingResult down = PriceScenario(downScenario);
//
//    return
//        (up.Value - down.Value)
//        /
//        (2.0 * bump);
//}
//
///**
// * @brief Estimates Rho using a central finite difference.
// *
// * Two Monte Carlo simulations are performed using bumped
// * continuously compounded risk-free interest rates. Common Random
// * Numbers (CRN) are used to reduce the variance of the finite
// * difference estimator.
// *
// * @param bump Absolute perturbation applied to the risk-free
// * interest rate.
// *
// * @return Estimated Rho.
// */
//[[nodiscard]]
//double Rho(double bump) const
//{
//    MarketScenario upScenario, downScenario;
//    upScenario = downScenario = m_marketScenario;
//
//    upScenario.RiskFreeRate += bump;
//    downScenario.RiskFreeRate -= bump;
//
//    PricingResult up = PriceScenario(upScenario);
//    PricingResult down = PriceScenario(downScenario);
//
//    return
//        (up.Value - down.Value)
//        /
//        (2.0 * bump);
//}
//
///**
// * @brief Estimates Theta using a central finite difference.
// *
// * Two Monte Carlo simulations are performed using shorter and
// * longer times to expiry. Common Random Numbers (CRN) are used to
// * reduce the variance of the finite difference estimator.
// *
// * @param bump Absolute perturbation applied to the time to expiry,
// * expressed in years.
// *
// * @return Estimated Theta.
// */
//[[nodiscard]]
//double Theta(double bump) const
//{
//    // NOTE:
//    //
//    // Theta constructs separate simulation grids for the bumped maturities.
//    // The number of time steps may therefore differ between the two scenarios.
//    // This is acceptable for the current implementation but may be revisited
//    // when the Risk module is introduced.
//    MarketScenario shorterScenario, longerScenario;
//    shorterScenario = longerScenario = m_marketScenario;
//
//    shorterScenario.TimeToMaturity -= bump;
//    longerScenario.TimeToMaturity += bump;
//
//    PricingResult shorter = PriceScenario(shorterScenario);
//    PricingResult longer = PriceScenario(longerScenario);
//
//    return
//        (shorter.Value - longer.Value)
//        /
//        (2.0 * bump);
//}

/**
         * @brief Computes option Delta.
         *
         * Delta measures the sensitivity of option value
         * with respect to changes in the underlying asset price.
         *
         * @return First derivative of price with respect to spot.
         */

//[[nodiscard]]
//double Delta() const
//{
//    if (m_volatility == 0.0)
//    {
//        const double terminalPrice = m_spot * std::exp(m_riskFreeRate * m_maturity);
//
//        // Delta = 1 if option is guaranteed to expire in the money.
//        // Delta = 0 otherwise.
//        return (terminalPrice > m_strike) ? 1.0 : 0.0;
//    }
//
//    const BlackScholesTerms terms = ComputeTerms();
//
//    switch (m_optionType)
//    {
//    case OptionType::Call:
//        return NormalCDF(terms.D1);
//
//    case OptionType::Put:
//        return NormalCDF(terms.D1) - 1.0;
//    }
//
//    assert(false && "Option type not supported");
//
//    return 0.0;
//}
//
///**
// * @brief Computes option Gamma.
// *
// * Gamma measures the sensitivity of Delta with respect to
// * changes in the underlying asset price. It is the second
// * derivative of the option value with respect to spot.
// *
// * @return Second derivative of price with respect to spot.
// */
//[[nodiscard]]
//double Gamma() const
//{
//    if (m_volatility == 0.0)
//    {
//        return 0.0;
//    }
//
//    const BlackScholesTerms terms = ComputeTerms();
//
//    return NormalPDF(terms.D1) / (m_spot * m_volatility * std::sqrt(m_maturity));
//}
//
///**
// * @brief Computes option Vega.
// *
// * Vega measures the sensitivity of the option value with
// * respect to changes in the volatility of the underlying asset.
// *
// * Vega is quoted per unit volatility (e.g. a change from
// * 20% to 21% corresponds to a volatility increase of 0.01).
// *
// * @return First derivative of price with respect to volatility.
// */
//[[nodiscard]]
//double Vega() const
//{
//    if (m_volatility == 0.0)
//    {
//        return 0.0;
//    }
//
//    const BlackScholesTerms terms = ComputeTerms();
//
//    return m_spot * NormalPDF(terms.D1) * std::sqrt(m_maturity);
//}
//
///**
// * @brief Computes option Theta.
// *
// * Theta measures the sensitivity of the option value with
// * respect to the passage of time while all other parameters
// * remain constant.
// *
// * Theta is quoted per year.
// *
// * @return First derivative of price with respect to time to maturity.
// */
//[[nodiscard]]
//double Theta() const
//{
//    if (m_volatility == 0.0)
//    {
//        return 0.0;
//    }
//
//    const BlackScholesTerms terms = ComputeTerms();
//
//    const double firstThetaTerm =
//        -m_spot * NormalPDF(terms.D1) * m_volatility
//        /
//        (2.0 * std::sqrt(m_maturity));
//
//    switch (m_optionType)
//    {
//    case OptionType::Call:
//        return firstThetaTerm
//            -
//            m_riskFreeRate * m_strike * std::exp(-m_riskFreeRate * m_maturity)
//            *
//            NormalCDF(terms.D2);
//
//    case OptionType::Put:
//        return firstThetaTerm
//            +
//            m_riskFreeRate * m_strike * std::exp(-m_riskFreeRate * m_maturity)
//            *
//            NormalCDF(-terms.D2);
//    }
//
//    assert(false && "Option type not supported");
//
//    return 0.0;
//}
//
///**
// * @brief Computes option Rho.
// *
// * Rho measures the sensitivity of the option value with
// * respect to changes in the continuously compounded
// * risk-free interest rate.
// *
// * Rho is quoted per unit interest rate (e.g. a change
// * from 5% to 6% corresponds to an interest rate increase of 0.01).
// *
// * @return First derivative of price with respect to the risk-free rate.
// */
//[[nodiscard]]
//double Rho() const
//{
//    if (m_volatility == 0.0)
//    {
//        return 0.0;
//    }
//
//    const BlackScholesTerms terms = ComputeTerms();
//
//    switch (m_optionType)
//    {
//    case OptionType::Call:
//        return
//            m_strike * m_maturity * std::exp(-m_riskFreeRate * m_maturity)
//            *
//            NormalCDF(terms.D2);
//
//    case OptionType::Put:
//        return
//            -m_strike * m_maturity * std::exp(-m_riskFreeRate * m_maturity)
//            *
//            NormalCDF(-terms.D2);
//    }
//
//    assert(false && "Option type not supported");
//
//    return 0.0;
//}