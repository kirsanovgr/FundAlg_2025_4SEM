add_library(lab1task5 include/vector.h src/main.cpp)

add_executable(tests5 tests/test_vector.cpp)
target_link_libraries(tests5 PRIVATE lab1task5 GTest::gtest_main)

add_test(NAME Test5 COMMAND tests5)