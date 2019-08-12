cmake_minimum_required (VERSION 2.8)
 
project (Demo1)
SET(SRC_LIST main.cpp)
# Find required protobuf package
find_package(Protobuf REQUIRED)
if(PROTOBUF_FOUND)
	message(STATUS "protobuf library found")
else()
	message(FATAL_ERROR "protobuf library is needed but cant be found")
endif()
include_directories(${PROTOBUF_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS Test.proto)
 
ADD_EXECUTABLE(Demo1 ${SRC_LIST} ${PROTO_SRCS} ${PROTO_HDRS})
 
target_link_libraries(Demo1 ${PROTOBUF_LIBRARIES})
