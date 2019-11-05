
CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

project(Test)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -m64 -Wall -g -ggdb -lpthread -fPIC")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -03 -m64 -w -lpthread")


set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")


#file(GLOB Src "${CMAKE_SOURCE_DIR}/*.cpp")
include_directories("${CMAKE_SOURCE_DIR}/")
AUX_SOURCE_DIRECTORY(. Src)

ADD_EXECUTABLE(${PROJECT_NAME} 		${Src}
								)
target_link_libraries(${PROJECT_NAME})
