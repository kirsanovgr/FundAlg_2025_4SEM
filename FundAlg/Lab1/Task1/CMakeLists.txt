add_library(Lab1Task1 include/container.hpp
        include/array.hpp
        src/array.cpp)

add_executable(Test1 tests/test_array.cpp)
target_link_libraries(Test1 PRIVATE Lab1Task1 GTest::gtest_main)

add_test(NAME TestArr COMMAND Test1)