
CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

project(TestService)

SET(CMAKE_CXX_STANDARD 11)

SET(CMAKE_CXX_FLAGS_DEBUG  "$ENV{CAMKEFLAGS} -O0 -Wall -lpthread -g -ggdb -m64 -fPIC")
SET(CMAKE_CXX_FLAGS_RELEASE  "$ENV{CAMKEFLAGS} -O3 -W  -lpthread  -m64 -fPIC")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
	SET(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/build/Release/bin")
	add_definitions(-DNDEBUG)
else()
	SET(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/build/Debug/bin")
	add_definitions(-DDEBUG)
endif()


SET(SDK_DIR  ${CMAKE_SOURCE_DIR}/../)


include_directories(${SDK_DIR}/Base/include)
include_directories(${SDK_DIR}/Server)
include_directories(${SDK_DIR}/NetService)

AUX_SOURCE_DIRECTORY(${SDK_DIR}/Server  SERVER_SRC)
AUX_SOURCE_DIRECTORY(${SDK_DIR}/NetService  NET_SRC)


AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/src SRC)

add_executable(${PROJECT_NAME} 
								${SRC} 
								${NET_SRC}
								${SERVER_SRC}
								)
							#target_link_libraries(${PROJECT_NAME} )
	

