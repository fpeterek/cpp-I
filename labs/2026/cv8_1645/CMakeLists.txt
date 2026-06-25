cmake_minimum_required(VERSION 3.8)
project(practice02)

set(CMAKE_CXX_STANDARD 26)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

add_compile_options(-Wall -Wextra -pedantic)

add_executable(main main.cpp)
