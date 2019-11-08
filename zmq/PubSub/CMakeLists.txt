cmake_minimum_required(VERSION 3.10)

project(TestZMQ)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_DEBUG_FLAGS "ENV{CMAKE_CXX_FLAGS} -lpthread -luuid")

include_directories(../zmq)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} 
						libzmq.a
						libczmq.a)
