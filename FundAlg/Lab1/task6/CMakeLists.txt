add_library(lab1task6 include/unique_ptr.h src/main.cpp)

add_executable(tests6 tests/test_unique_ptr.cpp)
target_link_libraries(tests6 PRIVATE lab1task6 GTest::gtest_main)

add_test(NAME Test6 COMMAND tests6)