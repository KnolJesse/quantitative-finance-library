add_library(QuantFinance
    Version.cpp
    Statistics/RunningStats.cpp
    Models/GeometricBrownianMotion.cpp
    Payoffs/EuropeanCallPayoff.cpp
    Payoffs/EuropeanPutPayoff.cpp
    Payoffs/ArithmeticAsianCallPayoff.cpp
    Payoffs/ArithmeticAsianPutPayoff.cpp
    Payoffs/DigitalCallPayoff.cpp
    Payoffs/DigitalPutPayoff.cpp
    Payoffs/GeometricAsianCallPayoff.cpp
    Payoffs/GeometricAsianPutPayoff.cpp
)

target_include_directories(QuantFinance
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)