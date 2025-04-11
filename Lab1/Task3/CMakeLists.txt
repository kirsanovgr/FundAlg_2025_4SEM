add_library(Lab1Task3 include/container.hpp
        include/list.hpp
        include/deque.hpp
        src/deque.cpp)

add_executable(Test3 tests/test_deque.cpp)
target_link_libraries(Test3 PRIVATE Lab1Task3 GTest::gtest_main)

add_test(NAME TestArr COMMAND Test3)