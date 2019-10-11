cmake_minimum_required(VERSION 3.10)

project(AyncSvr)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -luuid")

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")

AUX_SOURCE_DIRECTORY(. SRC_LIST)

add_executable(AyncSvr ${SRC_LIST})
target_link_libraries(AyncSvr 
								libzmq.a
								libczmq.a
								) 
