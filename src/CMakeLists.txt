add_library(QuantFinance)

target_include_directories(
    QuantFinance
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_sources(
    QuantFinance
    PRIVATE
        Version.cpp
)