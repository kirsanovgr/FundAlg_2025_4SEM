cmake_minimum_required(VERSION 3.10)
project(EX4)

add_executable(server src/server.c)
add_executable(client1 src/client1.c)
add_executable(client2 src/client2.c)

add_custom_target(run_all
    COMMAND ./server &
    COMMAND ./client1 &
    COMMAND ./client2 &
    DEPENDS server client1 client2
)