

cmake_minimum_required(VERSION 3.10)

project(Test)


set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -g -ggdb -lpthread -m64 -fPIC")

set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3  -lpthread -m64 -fPIC")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
	set(EXECUTABLE_OUTPUT_PATH  "${CMAKE_SOURCE_DIR}/build/Release/bin")
	add_definitions(-DNDEBUG)
else() 
	add_definitions(-DDEBUG)
	set(EXECUTABLE_OUTPUT_PATH  "${CMAKE_SOURCE_DIR}/build/Debug/bin")
endif()

AUX_SOURCE_DIRECTORY(. SRC)


add_executable(${PROJECT_NAME}
								${SRC})
