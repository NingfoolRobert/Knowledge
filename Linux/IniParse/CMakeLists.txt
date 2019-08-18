
cmake_minimum_required(VERSION 2.8)

project(main)

set(Boost_DEBUG ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULITITHREADED ON)


#include_directories(/usr/include ${PROJECT_SOURCE_DIR}/include)

include_directories(/usr/include ${PROJECT_SOURCE_DIR}/include)

#include_directories(${PROJECT_BINARY_PATH}/include)
#add_definitions($Boost_LIB_DIAGNOSTIC_DEFINITIONS)
#add_subdirectory(src)

set(BOOST_COMPONENTS  date_time chrono filesystem iostreams	program_options regex system thread unit_test_framework)

find_package(Boost REQUIRED COMPONENTS ${BOOST_COMPONENTS})

if(NOT Boost_FOUND)
	message("未发现Boost")
endif()

set(CMAKE_BUILD_TYPE "Debug")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS_DEBUG} -Wl,--rpath=./lib/ -m64 -Wall -o0 -g -ggdb -pthread -std=c++11 -fPIC")

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")

aux_source_directory(. SRC_LIST)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARIES})

add_executable(main ${SRC_LIST})
target_link_libraries(main  ${Boost_LIBRARIES})


