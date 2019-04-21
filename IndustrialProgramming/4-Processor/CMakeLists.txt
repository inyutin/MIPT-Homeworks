cmake_minimum_required(VERSION 3.9)
project(4-Processor)

set(CMAKE_CXX_STANDARD 14)
include_directories(${4-Processor_SOURCE_DIR}/Processor)

add_executable(4_processor main.cpp Processor/Commands.h Processor/StrSwitch.h Processor/GlobalVariables.h Processor/CodeToByte.h Processor/Text.h Processor/Jump.h)