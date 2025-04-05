add_library(lab1task3 src/main.cpp include/deque.h)

add_executable(tests3 tests/test_deque.cpp)
target_link_libraries(tests3 PRIVATE lab1task3 GTest::gtest_main)

add_test(NAME Test3 COMMAND tests3)
