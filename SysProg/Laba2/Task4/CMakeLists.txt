cmake_minimum_required(VERSION 3.22.1)
project(LabaInfa CXX)
set(CMAKE_CXX_STANDARD 20)
set(COMMON_FLAGS "-Wall -Wextra -Wpedantic -Werror")

add_executable(SPLaba2Task4
        main.cpp
        elevator_system.cpp
        elevator_system.hpp
        logger.cpp
        logger.hpp
        elevator.cpp
        elevator.hpp
)