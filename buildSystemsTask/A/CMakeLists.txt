cmake_minimum_required(VERSION 3.15)

project(lib)
add_library(lib INTERFACE)
target_sources(lib INTERFACE lib.h)
