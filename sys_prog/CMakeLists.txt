cmake_minimum_required(VERSION 3.22.1)
project(sys_prog C)

set(CMAKE_C_STANDARD 11)

add_compile_options(-Wall -Wextra -Werror -fno-omit-frame-pointer -pedantic -Wshadow -Wno-strict-prototypes)

add_subdirectory(lab1)