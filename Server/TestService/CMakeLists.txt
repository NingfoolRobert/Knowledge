
CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

project(TestService)

SET(CMAKE_CXX_STANDARD 11)

SET(CMAKE_CXX_FLAGS "-lpthread -D_REENTRANT")

SET(CMAKE_CXX_FLAGS_DEBUG  "$ENV{CAMKEFLAGS} -O0 -Wall  -g -ggdb -m64 -fPIC")
SET(CMAKE_CXX_FLAGS_RELEASE  "$ENV{CAMKEFLAGS} -O3 -W   -m64 -fPIC")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
	SET(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/build/Release/bin")
	add_definitions(-DNDEBUG)
else()
	SET(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/build/Debug/bin")
	add_definitions(-DDEBUG)
endif()


SET(SDK_DIR  ${CMAKE_SOURCE_DIR}/../../SDK)


include_directories(${SDK_DIR}/Base/include)
include_directories(${SDK_DIR}/Server/include)
#include_directories(${SDK_DIR}/NetService)
#include_directories(${SDK_DIR}/Other/Sqlite3/include)


link_directories(${SDK_DIR}/Other/Sqlite3/lib)
link_directories(${SDK_DIR}/Base/lib)
link_directories(${SDK_DIR}/Server/lib)

link_libraries(Base)
link_libraries(Service)
link_libraries(ObjService)
link_libraries(NetService)


AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/src SRC)

add_executable(${PROJECT_NAME} 
								${SRC}
								)

