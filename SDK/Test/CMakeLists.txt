

cmake_minimum_required(VERSION 3.10)

project("Test")

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_BUILD_TYPE "Release")

SET(CMAKE_CXX_FLAGS "$ENV{CMAKE_CXX_FLAGS} -O2 -pthread")



include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/Test)

aux_source_directory(${CMAKE_SOURCE_DIR}/src LIB_SRC)
aux_source_directory(${CMAKE_SOURCE_DIR}/Test TST_SRC)



add_executable(${PROJECT_NAME}  
					
					${LIB_SRC}
					${TST_SRC}
					)
