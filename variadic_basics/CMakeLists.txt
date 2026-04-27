cmake_minimum_required(VERSION 3.20)
project(test)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wconversion -pedantic")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)



add_executable(
    main
    main.cpp
)
