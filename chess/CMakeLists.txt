cmake_minimum_required(VERSION 3.5)

project(chess)

# Create a library named 'fsm'
add_executable(chess chess.c)
target_compile_definitions(chess PRIVATE TEST_CHESS)

set(CMAKE_GENERATOR "Ninja")
set(CMAKE_C_COMPILER "clang")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
#set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")


# It is composed of 1 header
target_include_directories(chess PUBLIC chess.h)
