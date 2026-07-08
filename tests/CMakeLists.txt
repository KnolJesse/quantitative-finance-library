add_executable(QuantFinanceTests 
    Statistics/RunningStatsTest.cpp
    Random/RandomGeneratorTest.cpp
    Models/GeometricBrownianMotionTest.cpp
    PathGeneration/PathTest.cpp
    PathGeneration/PathGeneratorTest.cpp
    Payoffs/EuropeanCallPayoffTest.cpp
    Pricing/MonteCarloPricerTest.cpp
    Pricing/BlackScholesPricerTest.cpp
    Pricing/PricingComparisonTest.cpp
    Pricing/BlackScholesGreeksTest.cpp
    Payoffs/EuropeanPutPayoffTest.cpp
    Pricing/MonteCarloGreeksTest.cpp
)

target_link_libraries(QuantFinanceTests
    PRIVATE
        QuantFinance
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(QuantFinanceTests)