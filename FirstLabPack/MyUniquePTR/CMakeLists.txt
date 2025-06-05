cmake_minimum_required(VERSION 3.28)
project(MyUniquePTR)

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

add_library(my_unique_ptr
        include/unique_ptr.hpp
        src/unique_ptr.cpp
)

target_include_directories(my_unique_ptr
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_options(my_unique_ptr PRIVATE ${COMMON_FLAGS})
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(my_unique_ptr PRIVATE ${COVERAGE_FLAGS})
    target_link_options(my_unique_ptr PRIVATE ${COVERAGE_FLAGS})
endif()

add_executable(unique_ptr_tests
        tests/unptrtests.cpp
)

target_compile_options(unique_ptr_tests PRIVATE ${COMMON_FLAGS})
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(unique_ptr_tests PRIVATE ${COVERAGE_FLAGS})
    target_link_options(unique_ptr_tests PRIVATE ${COVERAGE_FLAGS})
endif()

target_link_libraries(unique_ptr_tests
        PRIVATE my_unique_ptr
        PRIVATE GTest::gtest_main
)

gtest_discover_tests(unique_ptr_tests
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
            '${CMAKE_BINARY_DIR}/_deps/googletest-src/*'
            '*/usr/include/*'
            --output-file filtered.info
            --ignore-errors unused

            COMMAND ${GENHTML} filtered.info
            --output-directory coverage_report
            --title "${PROJECT_NAME} Coverage"

            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            DEPENDS unique_ptr_tests
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