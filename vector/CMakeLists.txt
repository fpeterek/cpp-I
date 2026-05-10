cmake_minimum_required(VERSION 3.8)

project(cpp-vector)

set(CMAKE_CXX_STANDARD 26)

add_compile_options(-Wall -Wextra -Wconversion -pedantic -Werror=switch)

add_executable(vector main.cpp)

