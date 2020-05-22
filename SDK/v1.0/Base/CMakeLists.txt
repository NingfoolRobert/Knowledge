cmake_minimum_required(VERSION 3.10)


project(Base)

SET(CMAKE_CXX_STANDARD 11)

set(BASE_VERSION_MAJOR 1)
set(BASE_VERSION_MINOR 0)
set(BASE_VERSION_PATCH 0)
set(BASE_VERSION_BUILD 1)
#set(Hello_world_VERSION_STR "\"${Hello_world_version_major}.${Hello_world_version_minor}.${Hello_world_version_patch}.${Hello_world_version_build}\"")

configure_file(
	"${CMAKE_SOURCE_DIR}/versionBase.h.in"
	"${CMAKE_SOURCE_DIR}/include/versionBase.h"		
	)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)

AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/src BASE_SRC)


add_library(Base STATIC ${BASE_SRC})
