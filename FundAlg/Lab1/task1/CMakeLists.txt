add_library(lab1task1 src/main.cpp include/array.h
        ../container.h)
target_include_directories(lab1task1 PUBLIC include)

add_executable(tests1 tests/test_array.cpp)
target_link_libraries(tests1 PRIVATE lab1task1 GTest::gtest_main)

add_test(NAME Test COMMAND tests1)

find_program(CPPCHECK cppcheck)
if(CPPCHECK)
    add_custom_target(cppcheck
            COMMAND ${CPPCHECK}
            --enable=all
            --inconclusive
            --quiet
            --suppress=missingIncludeSystem
            -I include
            src
            tests
            COMMENT "Запуск cppcheck..."
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            VERBATIM
    )
else()
    message(WARNING "cppcheck не найден, цель 'cppcheck' недоступна.")
endif()
