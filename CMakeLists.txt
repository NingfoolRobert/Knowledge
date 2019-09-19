cmake_minimum_required(VERSION 3.10)

project(Svr)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BUILD_TYPE "Debug")
#set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} -Wl,--rpath=./lib/ -m64 -Wall -g -ggdb -lpthread")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_RELEASE} -O3 -Wall -Wno -unused-but-set-variable")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")

#aux_source_directory(. SRC_LIST)
AUX_SOURCE_DIRECTORY(. SRC_LIST)
add_executable(Svr ${SRC_LIST})
target_link_libraries(Svr uuid libczmq.a libzmq.a)
