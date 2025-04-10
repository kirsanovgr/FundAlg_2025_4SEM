cmake_minimum_required(VERSION 3.20)
project(MyProject)
set(CMAKE_CXX_STANDARD 20)


set(COMMON_FLAGS
        -Wall
        -Wextra
        -Wpedantic
        -Werror
)

add_library(liblogger
        include/logger.hpp
        src/logger.cpp)


add_executable(ex1 tests/test_logger.cpp)

target_link_libraries(ex1 liblogger)