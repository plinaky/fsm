cmake_minimum_required(VERSION 3.1)

project(
  fsm
  LANGUAGES C)

# Create a library named 'fsm'
add_library(fsm INTERFACE)

# It is composed of 1 header
target_sources(fsm INTERFACE fsm.h)

