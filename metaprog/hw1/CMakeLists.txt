cmake_minimum_required(VERSION 3.15)
project(cpp_test)

set(CMAKE_CXX_STANDARD 17)

# Local header files here ONLY
SET(TARGET_H
        csv.h
        )

add_executable(cpp_test main.cpp)