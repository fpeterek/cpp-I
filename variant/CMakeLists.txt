cmake_minimum_required(VERSION 3.8)

project(cpp-variant)

set(CMAKE_CXX_STANDARD 26)

add_compile_options(-Wall -Wextra -pedantic -Wconversion)

add_executable(main main.cpp)

