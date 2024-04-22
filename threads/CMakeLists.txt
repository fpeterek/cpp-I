cmake_minimum_required(VERSION 3.20)
project(test)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-Wall -Werror -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include_directories(./include /usr/include)

add_executable(
    test
    main.cpp
)
