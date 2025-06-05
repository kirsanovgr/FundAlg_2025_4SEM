add_library(Lab2Task2 include/bigint.hpp
        src/bigint.cpp)

add_executable(Test2.2 tests/test_bigint.cpp)
target_link_libraries(Test2.2 PRIVATE Lab2Task2 GTest::gtest_main)

add_test(NAME Test COMMAND Test2.2)