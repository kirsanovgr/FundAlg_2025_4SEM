add_library(lab1task4 include/stack.h src/main.cpp)

add_executable(tests4 tests/test_stack.cpp)
target_link_libraries(tests4 PRIVATE lab1task4 GTest::gtest_main)

add_test(NAME Test4 COMMAND tests4)