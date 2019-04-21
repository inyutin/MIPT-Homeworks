cmake_minimum_required(VERSION 3.12)
project(6-Differantiator)

set(CMAKE_CXX_STANDARD 14)
include_directories(${6-Differantiator_SOURCE_DIR}/include)
include_directories(${6-Differantiator_SOURCE_DIR}/src)

add_executable(6-Differantiator main.cpp src/Derivative.cpp include/Derivative.hpp include/Text.hpp src/Text.cpp)