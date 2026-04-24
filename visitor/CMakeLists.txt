cmake_minimum_required(VERSION 3.8)

project(cpp-visitor)

set(CMAKE_CXX_STANDARD 23)

add_compile_options(-Wall -Wextra -Wconversion -pedantic -Werror=switch)

add_executable(visitor main.cpp)

