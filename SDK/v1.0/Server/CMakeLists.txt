

cmake_minimum_required(VERSION 3.10)

project(SDK)

SET(CMAKE_CXX_STANDARD 11)
set(OBJECT_VERSION_MAJOR 1)
set(OBJECT_VERSION_MINOR 0)
set(OBJECT_VERSION_PATCH 0)
set(OBJECT_VERSION_BUILD 1)
set(OBJECT_VERSION_VALUE "\"${OBJECT_VERSION_MAJOR}.${OBJECT_VERSION_MINOR}.${OBJECT_VERSION_PATCH}.${OBJECT_VERSION_BUILD}\"")

configure_file(
	"${CMAKE_SOURCE_DIR}/versionObject.h.in"
	"${CMAKE_SOURCE_DIR}/include/versionObject.h"		
	)




SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -g -ggdb  -Wall -lphtread")

SET(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/../Base/include)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)



AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/src		SVR_SRC)
AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/../Base/src		BASE_SRC)

add_library(ObjService STATIC	${BASE_SRC} ${SVR_SRC})
