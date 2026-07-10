#pragma once 

namespace qf
{
    /**
     * @brief Container for the result of a pricing calculation.
     *
     * Stores the estimated present value of a financial instrument
     * and an associated statistical uncertainty measure.
     *
     * For deterministic pricing engines, StandardError is typically
     * zero. For stochastic pricing engines such as Monte Carlo engines,
     * StandardError represents the standard error of the estimated
     * price.
     */
	struct PricingResult
	{
	public:
        /**
         * @brief Present value of the priced instrument.
         */
		double Value = 0.0;

        /**
         * @brief Statistical uncertainty of the price estimate.
         *
         * For Monte Carlo pricing engines, this represents the
         * standard error of the estimator after discounting.
         */
		double StandardError = 0.0; 
	};
}