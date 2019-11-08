

cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)

project(EmailDamon)


set(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Wall -m64  -g -ggdb -lpthread")

AUX_SOURCE_DIRECTORY(. SRC)

add_executable(${PROJECT_NAME} ${SRC})
