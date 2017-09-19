cmake_minimum_required(VERSION 3.8)
project(multi_precision_ints)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS -Wall)

set(SOURCE_FILES main.cpp multiprecision_integer.h)
add_executable(multi_precision_ints ${SOURCE_FILES})