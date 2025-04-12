cmake_minimum_required(VERSION 3.10)
project(Laba_1_6 C)

set(CMAKE_C_STANDARD 11)

add_executable(server server.c server_utility.c server_utility.h errors.c errors.h)


add_executable(client client.c client.h
        utility.c
        utility.h errors.c errors.h)