CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

project(AlarmService)

set(CMAKE_CXX_STANDARD 11)


set(CMAKE_BUILD_TYPE "Debug")
#set(CMAKE_BUILD_TYPE "Release")

set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -m64 -Wall -g -ggdb  -lpthread -std=c++11 -fPIC")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -m64 -w -lpthread -std=c++11 -fPIC")

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")


include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/CICCTools)
include_directories(${PROJECT_SOURCE_DIR}/include/GTool)
link_directories(${PROJECT_SOURCE_DIR}/lib)
link_libraries(${PROJECT_SOURCE_DIR}/lib)

AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/src  SRC)
AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/include/CICCTools libSrc)
AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/include/GTool gSrc)
add_executable(${PROJECT_NAME}  
								${SRC}
					 			${libSrc}
								${gSrc}
								) 
target_link_libraries(${PROJECT_NAME}
						
					 	libjsoncpp.a

						)
