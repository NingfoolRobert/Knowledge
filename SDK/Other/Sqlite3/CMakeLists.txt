
cmake_minimum_required(VERSION 3.10)

project(Sqlite3)


SET(CMAKE_BUILD_TYPE "Release")

SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

SET(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/lib")



include_directories(${CMAKE_SOURCE_DIR}/include)
aux_source_directory(${CMAKE_SOURCE_DIR}/src/ SRC)

link_libraries(dl)

add_library(Sqlite3 STATIC ${SRC})






