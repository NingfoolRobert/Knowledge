cmake_minimum_required(VERSION 3.10)



project(TestMySql)

SET(CMAKE_CXX_STANDARD 11)

SET(CMAKE_CXX_FLAGS "-lpthread -D_REENTRANT")

SET(CMAKE_CXX_FLAGS_DEBUG  "$ENV{CAMKEFLAGS} -O0 -Wall  -g -ggdb -m64 -fPIC")
SET(CMAKE_CXX_FLAGS_RELEASE  "$ENV{CAMKEFLAGS} -O3 -W   -m64 -fPIC")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
	SET(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/build/Release/bin")
	add_definitions(-DNDEBUG)
else()
	SET(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/build/Debug/bin")
	add_definitions(-DDEBUG)
endif()


SET(MYSQL_DIR /home/nbf/mysql++)

include_directories(${MYSQL_DIR}/include)
include_directories(${MYSQL_DIR}/mysql)
include_directories(/usr/include/mysql)
include_directories(${CMAKE_SOURCE_DIR}/src)



aux_source_directory(${CMAKE_SOURCE_DIR}/src SRC)

link_directories(${MYSQL_DIR}/lib)
link_directories(/usr/lib64/mysql)
#set(Hello_world_version_major 1)
#set(Hello_world_version_minor 0)
#set(Hello_world_version_patch 0)
#set(Hello_world_version_build 1)
##set(Hello_world_VERSION_STR "\"${Hello_world_version_major}.${Hello_world_version_minor}.${Hello_world_version_patch}.${Hello_world_version_build}\"")
#
#configure_file(
#   "${HELLO_WORLD_SOURCE_DIR}/helloConfig.h.in"
#   "${HELLO_WORLD_BINARY_DIR}/helloConfig.h"		
#	)
#set(SRC_LIST Test.cpp)

#include_directories("${HELLO_WORLD_BINARY_DIR}")

link_libraries("libmysqlpp.a")

add_executable(${PROJECT_NAME}  
								${SRC}
								)

target_link_libraries(${PROJECT_NAME} 
			-lmysqlclient
								)
