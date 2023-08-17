cmake_minimum_required(VERSION 3.15)

project(windows-bricks)

# use c++20 for concept/requires
# use c++23 for std::string.resize_and_overwrite
set(CMAKE_CXX_STANDARD 23)
# hide symbols in libs by default
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

add_library(exit_event STATIC src/exit_event.cpp)
