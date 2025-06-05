cmake_minimum_required(VERSION 3.28)
project(BigInt2)

set(CMAKE_CXX_STANDARD 20)

enable_testing()

set(COMMON_FLAGS
        -Wall
        -Wextra
        -Wpedantic
        -Werror
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(COVERAGE_FLAGS
            --coverage
            -fprofile-arcs
            -ftest-coverage
    )
endif()

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

add_library(my_bigint
        include/bigint.hpp
        src/bigint.cpp
)

target_include_directories(my_bigint
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_options(my_bigint PRIVATE ${COMMON_FLAGS})
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(my_bigint PRIVATE ${COVERAGE_FLAGS})
    target_link_options(my_bigint PRIVATE ${COVERAGE_FLAGS})
endif()

add_executable(bigint_tests
        tests/test.cpp
)

target_compile_options(bigint_tests PRIVATE ${COMMON_FLAGS})
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(bigint_tests PRIVATE ${COVERAGE_FLAGS})
    target_link_options(bigint_tests PRIVATE ${COVERAGE_FLAGS})
endif()

target_link_libraries(bigint_tests
        PRIVATE my_bigint
        PRIVATE GTest::gtest_main
)

gtest_discover_tests(bigint_tests
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

find_program(LCOV lcov)
find_program(GENHTML genhtml)

if(LCOV AND GENHTML)
    add_custom_target(coverage
            COMMAND ${LCOV} --capture --directory . --output-file full.info
            --rc branch_coverage=1
            --ignore-errors mismatch

            COMMAND ${LCOV} --remove full.info
            '/mnt/d/FundAlg2/BigInt2/cmake-build-debug/_deps/googletest-src/*'
            '*/usr/include/*'
            '*/c++/13/*
            '*/c++/*/bits/*'
            --output-file filtered.info
            --ignore-errors unused

            COMMAND ${GENHTML} filtered.info
            --output-directory coverage_report
            --title "${PROJECT_NAME} Coverage"

            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            DEPENDS bigint_tests
            COMMENT "Generating coverage report..."
            VERBATIM
    )
endif()

find_program(CPPCHECK cppcheck)
if(CPPCHECK)
    add_custom_target(cppcheck
            COMMAND ${CPPCHECK}
            --enable=all
            --suppress=missingIncludeSystem
            --inline-suppr
            --quiet
            -I ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${CMAKE_CURRENT_SOURCE_DIR}/src
            COMMENT "Запуск cppcheck..."
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            VERBATIM
    )
endif()