cmake_minimum_required(VERSION 3.22)
project(4 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(4
    4.h
    client.c
    server.c)
