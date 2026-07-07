add_library(QuantFinance
    Version.cpp
    Statistics/RunningStats.cpp
    Models/GeometricBrownianMotion.cpp
)

target_include_directories(QuantFinance
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)