cmake_minimum_required(VERSION 3.10)
project(network-analyzer)

set(CMAKE_CXX_STANDARD 11)

add_executable(network-analyzer source/main.cpp source/SystemInfo.cpp source/NetworkInfo.cpp)
