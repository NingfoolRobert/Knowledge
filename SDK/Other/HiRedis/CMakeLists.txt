
cmake_minimum_required(VERSION 3.10)


project(HiRedis)

set(CMAKE_CXX_STANDARD 11)


set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)


include_directories(${CMAKE_SOURCE_DIR}/include)

aux_source_directory(${CMAKE_SOURCE_DIR}/src HIREDIS_SRC)


add_library(HiRedis STATIC ${HIREDIS_SRC})
