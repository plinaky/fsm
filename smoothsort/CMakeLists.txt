cmake_minimum_required(VERSION 3.5)

project(smoothsort)

#add_library(big_stack OBJECT big_stack.c)
add_executable(smoothsort smoothsort.c)

set(CMAKE_GENERATOR "Ninja")
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

