cmake_minimum_required(VERSION 3.8)
project(practice04)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -stdlib=libc++ -lc++abi")
set(CMAKE_CXX_COMPILER "clang++")

add_compile_options(-Wall -Wextra -pedantic)

add_executable(main main.cpp)
