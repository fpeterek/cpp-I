cmake_minimum_required(VERSION 3.8)

project(tcp)

set(CMAKE_CXX_STANDARD 26)

add_compile_options(-Wall -Wextra -Wconversion -pedantic -Werror=switch)

link_libraries(
    -lsfml-network
)

add_executable(linux_server linux_server.cpp)

add_executable(linux_client linux_client.cpp)

add_executable(sf_client sf_client.cpp)

add_executable(sf_server sf_server.cpp)

