add_library(QuantFinance
    Version.cpp
    Statistics/RunningStats.cpp
    Models/GeometricBrownianMotion.cpp
    Payoffs/EuropeanCallPayoff.cpp
    Payoffs/EuropeanPutPayoff.cpp
)

target_include_directories(QuantFinance
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)