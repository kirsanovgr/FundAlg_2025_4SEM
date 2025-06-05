cmake_minimum_required(VERSION 3.22)
project(elev)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(elev
        main.cpp
        src/logger.cpp
        src/el.cpp
)

target_include_directories(elev PRIVATE include)
