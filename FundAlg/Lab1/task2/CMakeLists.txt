add_library(lab1task2 include/list.h src/main.cpp)

add_executable(tests2 tests/test_list.cpp)
target_link_libraries(tests2 PRIVATE lab1task2 GTest::gtest_main)

add_test(NAME Test2 COMMAND tests2)
