
CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

project("TestCurl")

SET(CMAKE_CXX_STANDARD 11)

SET(CMAKE_CXX_FLAGS "-lpthread -pthread -lz -ldl -D_REENTRANT")

SET(CMAKE_CXX_FLAGS_DEBUG  "$ENV{CAMKEFLAGS} -O0 -Wall   -g -ggdb -m64 -fPIC")
SET(CMAKE_CXX_FLAGS_RELEASE  "$ENV{CAMKEFLAGS} -O2 -W   -m64 -fPIC")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
	SET(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/build/Release/bin")
	add_definitions(-DNDEBUG)
else()
	SET(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/build/Debug/bin")
	add_definitions(-DDEBUG)
endif()


SET(SDK_DIR  /home/nbf/github/Knowledge/SDK/v1.0)


include_directories(${SDK_DIR}/Base/include)
include_directories(${SDK_DIR}/Server/include)
include_directories(${SDK_DIR}/../Other)
include_directories(${SDK_DIR}/../Other/hffix)
include_directories(${SDK_DIR}/../Other/curl/include)
include_directories(${SDK_DIR}/../Other/OpenSSL/include)
include_directories(${SDK_DIR}/../Other/OpenSSL)
#include_directories(${CMAKE_SOURCE_DIR}/zmq)
#include_directories(${CMAKE_SOURCE_DIR}/rapidFIXML)
#
AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/src SRC)
#AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/rapidFIXML RAPIDFIXMLSRC)
#AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/zmq	ZMQ_SRC)

#AUX_SOURCE_DIRECTORY(${SDK_DIR}/Server/src SDK_SVR)
#AUX_SOURCE_DIRECTORY(${SDK_DIR}/Base/src SDK_BASE)

link_directories(${SDK_DIR}/Server/lib)
link_directories(${PB_DIR}/lib)
link_directories(${SDK_DIR}/../Other/curl/lib)
link_directories(${SDK_DIR}/../Other/OpenSSL/lib)
link_libraries("libObjService.a")
#link_libraries("libPBDataFeed.a")
link_libraries("libcurl.a")
link_libraries("libssl.a")
link_libraries("libcrypto.a")
add_executable(${PROJECT_NAME} 
	#							${SDK_BASE}
	#							${SDK_SVR}
	#								${ETL_SRC}
	#							${ZMQ_SRC}
								${SRC}
	#							${RAPIDFIXMLSRC}
  								)

target_link_libraries(${PROJECT_NAME} 
	
	 							libczmq.a
								libzmq.a
								libprotobuf.a
								libuuid.a)
