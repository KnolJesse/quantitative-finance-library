add_executable(QuantFinanceTests
    VersionTest.cpp
    RunningStatsTest.cpp
    RandomGeneratorTest.cpp
    GeometricBrownianMotionTest.cpp
    PathTest.cpp
    PathGeneratorTest.cpp
    EuropeanCallPayoffTest.cpp
    MonteCarloPricerTest.cpp
    BlackScholesPricerTest.cpp
    PricingComparisonTest.cpp
)

target_link_libraries(QuantFinanceTests
    PRIVATE
        QuantFinance
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(QuantFinanceTests)