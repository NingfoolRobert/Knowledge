

cmake_minimum_required(VERSION 3.10)

project("AgentService")


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

set(Resource ${CMAKE_SOURCE_DIR}/../../Resource)
set(Proto ${CMAKE_SOURCE_DIR}/../../proto)
include_directories(${CMAKE_SOURCE_DIR}/include)
AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/src SRCITEM)

include_directories(${Resource}/Server)
AUX_SOURCE_DIRECTORY(${Resource}/Server ResSvr)
include_directories(${Resource}/CICCTools)
AUX_SOURCE_DIRECTORY(${Resource}/CICCTools ResCICCTools)

include_directories(${Proto}/)
include_directories(${Proto}/Msg/)
include_directories(${Proto}/Msg/util/)
include_directories(${Proto}/Msg/util/bean)
include_directories(${Proto}/Msg/util/msg)

link_libraries(${Proto}/lib)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(PBLIBPATH ${Proto}/lib/libPBDataFeed_D.a)
elseif ( CMAKE_BUILD_TYPE STREQUAL "Release" )
	set(PBLIBPATH ${Proto}/lib/libPBDataFeed.a)
endif()

#AUX_SOURCE_DIRECTORY(. SRC)


add_executable(${PROJECT_NAME}
								${ResSvr}
								${ResCICCTools}
								${SRCITEM})

target_link_libraries(${PROJECT_NAME} 
	
								libczmq.a
								libzmq.a
								${PBLIBPATH}
								libprotobuf.a
								libuuid.a)
