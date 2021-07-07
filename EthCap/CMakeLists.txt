cmake_minimum_required(VERSION 3.19)
project(EthCap)

set(CMAKE_CXX_STANDARD 14)
add_definitions("-Wall -lpthread -g")
add_executable(EthCap main.cpp)
target_link_libraries(EthCap /usr/lib/x86_64-linux-gnu/libpcap.so
        )