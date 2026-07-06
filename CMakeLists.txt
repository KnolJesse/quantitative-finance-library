cmake_minimum_required(VERSION 3.28)

project(QuantFinance LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(QuantFinance)

target_sources(
    QuantFinance
    PRIVATE
        src/Version.cpp
)

target_include_directories(
    QuantFinance
    PUBLIC
        include
		
)