

cmake_minimum_required(VERSION 3.10)



project(SDK)

SET(CMAKE_CXX_STANDARD 11)

SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O3 -Wall -lphtread")

SET(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/../Base/include)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)



AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/src/Service		SVR_SRC)
AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/src/ObjectService		OBJ_SRC)
AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/src/Net			NET_SRC)
AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/../Base/src		BASE_SRC)

add_library(Service STATIC		${SVR_SRC} ${BASE_SRC})
add_library(ObjService STATIC	${OBJ_SRC} ${SVR_SRC} ${BASE_SRC})
add_library(NetService STATIC	${NET_SRC} ${SVR_SRC} ${OBJ_SRC} ${BASE_SRC})
