cmake_minimum_required(VERSION 3.12)
project(3_Stack)

set(CMAKE_CXX_STANDARD 14)
include_directories(${3_Stack_SOURCE_DIR}/Include)

add_executable(3_Stack main.cpp HardStack.hpp)