add_executable(QuantFinanceTests
    VersionTest.cpp
    RunningStatsTest.cpp
    RandomGeneratorTest.cpp
)

target_link_libraries(QuantFinanceTests
    PRIVATE
        QuantFinance
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(QuantFinanceTests)