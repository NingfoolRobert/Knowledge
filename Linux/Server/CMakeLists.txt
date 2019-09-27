cmake_minimum_required(VERSION 3.10)

project(TestService)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_TYEP "Debug")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_DEBUG_FLAGS} -Wall -g -ggdb -std=c++11")


AUX_SOURCE_DIRECTORY(. SRC_LIST)

add_executable(TestService SRC_LIST)


