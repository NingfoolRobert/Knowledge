cmake_minimum_required(VERSION 3.11)

project("cppcheckTest")


#cppcheck
#find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)
#if(CMAKE_CXX_CPPCHECK)
#	list(
#		APPEND CMAKE_CXX_CPPCHECK 
#		"--enable=warning"
#		"--inconclusive"
#		"--force"
#		"--inline-suppr"
#		#		"--suppressions-list = ${CMAKE_SOURCE_DIR}/CppCheckSuppressions.txt"
#		)
#endif()

# 设定cmake模块的位置
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
#CppCheck
set(CPPCHECK_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/cppcheck")
file(MAKE_DIRECTORY ${CPPCHECK_OUTPUT_DIRECTORY})
find_package(cppcheck)
set(CPPCHECK_QUIET_ARG --report-progress)
set(CPPCHECK_QUIET_ARG "")
include(CppcheckTargets)
set(CPPCHECK_EXTRA_ARGS -j 8 -v --max-configs=1 --xml --xml-version=2
	--std=c++1y --platform=unix64 --enable=warning)

add_executable(${PROJECT_NAME}  "main.cpp")
