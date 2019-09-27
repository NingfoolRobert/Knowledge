cmake_minimum_required(VERSION 3.10)

project(TestService)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_TYEP "Debug")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} -Wl,--rpath=./lib/ -m64 -Wall -g -ggdb -lpthread -std=c++11")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")

AUX_SOURCE_DIRECTORY(. SRC_LIST)

add_executable(TestService ${SRC_LIST})


