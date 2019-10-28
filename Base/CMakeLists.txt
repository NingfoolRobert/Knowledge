
CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

project(Test)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -m64 -Wall -g -ggdb -lpthread -fPIC")



set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")



#file(GLOB Src "${CMAKE_SOURCE_DIR}/*.cpp")

AUX_SOURCE_DIRECTORY(. Src)

add_executable(${PROJECT_NAME} 
								${Src}
								)
