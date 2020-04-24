cmake_minimum_required(VERSION 3.10)


SET(CMAKE_CXX_STANDARD 11)

project(TestService)

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG  "$ENV{CAMKEFLAGS} -O0 -Wall -lpthread -g -ggdb -m64 -fPIC")
SET(CMAKE_CXX_FLAGS_RELEASE  "$ENV{CAMKEFLAGS} -O3 -W  -lpthread  -m64 -fPIC")

SET(SDK_DIR ${CMAKE_SOURCE_DIR}/../)
include_directories(${SDK_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/../../Base/include)


link_directories(${SDK_DIR}/lib)
link_libraries("libObjService.a")

aux_source_directory(${CMAKE_SOURCE_DIR}/src  SRC)



add_executable (${PROJECT_NAME} 
								${SRC}
								)
