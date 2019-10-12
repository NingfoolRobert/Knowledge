
CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

project(TestJson)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} -m64 -Wall -g -ggdb  -lpthread -std=c++11 -fPIC")


set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")


include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)
link_libraries(${PROJECT_SOURCE_DIR}/lib)

AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/src  SRC)

add_executable(${PROJECT_NAME}  ${SRC})
target_link_libraries(${PROJECT_NAME}
						
						libjsoncpp.a

						)
