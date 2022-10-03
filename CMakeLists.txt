cmake_minimum_required(VERSION 3.10)

project(VierGewinnt)

set(SDL2_DIR ${CMAKE_CURRENT_LIST_DIR})
find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

add_executable(VierGewinnt ./src/main.cpp ./src/VierGewinnt.h)
target_link_libraries(VierGewinnt ${SDL2_LIBRARIES})