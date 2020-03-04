
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
include_directories(${SDK_DIR}/Server)
include_directories(${SDK_DIR}/Other/Sqlite3/include)


link_directories(${SDK_DIR}/Other/Sqlite3/lib)



AUX_SOURCE_DIRECTORY(${SDK_DIR}/Server  SERVER_SRC)


AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/src SRC)

add_executable(${PROJECT_NAME} 
								${SRC}
								${SERVER_SRC}
								)
							#target_link_libraries(${PROJECT_NAME} )
	

