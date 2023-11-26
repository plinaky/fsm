cmake_minimum_required(VERSION 3.5)

project(big_stack)

# Create a library named 'fsm'
#add_library(big_stack OBJECT big_stack.c)
add_executable(test_stack big_stack.c)
target_compile_definitions(test_stack PRIVATE TEST_BIG_STACK)

set(CMAKE_GENERATOR "Ninja")
set(CMAKE_C_COMPILER "clang")


# It is composed of 1 header
target_include_directories(test_stack PUBLIC big_stack.h)
