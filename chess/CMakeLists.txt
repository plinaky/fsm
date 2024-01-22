cmake_minimum_required(VERSION 3.5)

project(chess)

add_executable(chess chess.c board.c)

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Os")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -S")
#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)
