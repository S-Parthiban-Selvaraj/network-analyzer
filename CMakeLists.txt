cmake_minimum_required(VERSION 3.10)
project(network-probe)

set(CMAKE_CXX_STANDARD 11)

add_executable(network-probe source/main.cpp source/SystemInfo.cpp)
