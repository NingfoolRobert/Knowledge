
cmake_minimum_required(VERSION 3.10)


project("TMySqlSvr")

SET(CMAKE_CXX_STANDARD 11)

SET(EXECUTABLE_OUTPUT_PATH  "${CMAKE_SOURCE_DIR}/build/Debug/bin")

SET(CMAKE_CXX_DEBUG_FLAGS "${CMAKE_CXX_FLAGS} -O0 -gdb -pthread")
SET(CMAKE_CXX_RELEASE_FLAGS "${CMAKE_CXX_FLAGS} -O2 -pthread")



