cmake_minimum_required(VERSION 3.20)
project(LabsFA2 CXX)

set(CMAKE_CXX_STANDARD 20)

enable_testing()

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_compile_options(-Wall -Wextra -Werror -Wpedantic)

add_subdirectory(FundAlg/Lab1/Task1)
add_subdirectory(FundAlg/Lab1/Task2)
add_subdirectory(FundAlg/Lab1/Task3)