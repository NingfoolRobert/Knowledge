cmake_minimum_required(VERSION 3.10)



project(HELLO_WORLD)


set(Hello_world_version_major 1)
set(Hello_world_version_minor 0)
set(Hello_world_version_patch 0)
set(Hello_world_version_build 1)
#set(Hello_world_VERSION_STR "\"${Hello_world_version_major}.${Hello_world_version_minor}.${Hello_world_version_patch}.${Hello_world_version_build}\"")

configure_file(
   "${HELLO_WORLD_SOURCE_DIR}/helloConfig.h.in"
   "${HELLO_WORLD_BINARY_DIR}/helloConfig.h"		
	)
set(SRC_LIST Test.cpp)

include_directories("${HELLO_WORLD_BINARY_DIR}")


add_executable(hello ${SRC_LIST})


