cmake_minimum_required(VERSION  3.10)

project(Test)


set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_TYPE "Debug")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_DEBUG_FALGS} -Wall -pthread -g -ggdb ")
set(EXECUTABLE_OUTPUT_PATH,"${PROJECT_BINARY_DIR}/bin")


AUX_SOURCE_DIRECTORY(. SRC_LIST)

add_executable(Test ${SRC_LIST})


