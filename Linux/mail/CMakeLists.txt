CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

project(AlarmService)

set(CMAKE_CXX_STANDARD 11)


set(CMAKE_BUILD_TYPE "Debug")
#set(CMAKE_BUILD_TYPE "Release")

set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -m64 -Wall -g -ggdb  -lpthread -std=c++11 -fPIC")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -m64 -w -lpthread -std=c++11 -fPIC")

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")

add_definitions( -Wno-sign-compare )

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/CICCTools)
include_directories(${PROJECT_SOURCE_DIR}/include/Server)
include_directories(${PROJECT_SOURCE_DIR}/include/Server/Handler)
include_directories(${PROJECT_SOURCE_DIR}/include/Server/Subscribe)
include_directories(${PROJECT_SOURCE_DIR}/proto/Msg)
include_directories(${PROJECT_SOURCE_DIR}/proto)

link_directories(${PROJECT_SOURCE_DIR}/lib)
link_libraries(${PROJECT_SOURCE_DIR}/lib)

AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/src  SRC)
AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/include/CICCTools libSrc)
AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/include/Server gSrc)

AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/include/Server/Handler UserSrc)

AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/include/Server/Subscribe SubSrc)
#AUX_SOURCE_DIRECTORY(%{CMAKE_SOURCE_DIR}/proto/Msg/*.cc  PBSrc)
file(GLOB PBSrc "${CMAKE_SOURCE_DIR}/proto/Msg/*.cc")


add_executable(${PROJECT_NAME}  
 								${SRC} 
					 			${libSrc}
								${gSrc}
								${UserSrc}
								${SubSrc}
								${PBSrc}
 								)     
target_link_libraries(${PROJECT_NAME}
						
					 	libjsoncpp.a
						libczmq.a
						libzmq.a
						libuuid.a
						libprotobuf.a
						)
