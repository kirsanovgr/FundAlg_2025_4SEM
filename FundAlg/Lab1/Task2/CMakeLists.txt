add_library(Lab1Task2 include/container.hpp
        include/list.hpp
        src/list.cpp)

add_executable(Test2 tests/test_list.cpp)
target_link_libraries(Test2 PRIVATE Lab1Task2 GTest::gtest_main)

add_test(NAME TestArr COMMAND Test2)