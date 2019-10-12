


cmake_minimum_required(VERSION 2.8)

project(Test)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} -Wl,--rpath=./lib/ -m64 -Wall -g -ggdb -lpthread -std=c++11 -fPIC")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")

aux_source_directory(. SRC_LIST)
add_executable(Test ${SRC_LIST})



