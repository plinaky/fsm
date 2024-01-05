cmake_minimum_required(VERSION 3.5)

project(chess)

add_executable(chess chess.c board.c move.c)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Ofast")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -S")
set(CMAKE_BUILD_TYPE Release)
