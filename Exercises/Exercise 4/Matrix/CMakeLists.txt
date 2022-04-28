cmake_minimum_required(VERSION 3.15)
project(Matrix)

set(CMAKE_CXX_STANDARD 17)

add_executable(Matrix main.cpp Activation.cpp Activation.h Matrix.cpp Matrix.h Dense.cpp Dense.h MlpNetwork.cpp MlpNetwork.h)