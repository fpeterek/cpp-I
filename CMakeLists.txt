cmake_minimum_required(VERSION 3.8)

project(cpp-templates)

set(CMAKE_CXX_STANDARD 23)

add_compile_options(-Wall -Wextra -pedantic -Werror)

add_executable(templates main.cpp)

