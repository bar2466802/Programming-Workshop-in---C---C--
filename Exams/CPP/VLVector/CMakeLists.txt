cmake_minimum_required(VERSION 3.15)
project(VLVector)

set(CMAKE_CXX_STANDARD 14)

add_executable(VLVector main.cpp)

FILE (GLOB_RECURSE clion_all_headers ${CMAKE_SOURCE_DIR}/*.hpp)
FILE (GLOB_RECURSE clion_all_heades ${CMAKE_SOURCE_DIR}/*.h)
ADD_CUSTOM_TARGET(all_clion
        SOURCES ${clion_all_headers}
        SOURCES ${clion_all_heades}
        )