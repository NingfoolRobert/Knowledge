cmake_minimum_required(VERSION 3.11)


project("TestProtoCMake")

FIND_PACKAGE(Protobuf  REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})


set(TOP_DIR  ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(${CMAKE_SOURCE_DIR}/PB)
set_source_files_properties(${PROTO_SRC} ${PROTO_HEADER} PROPERTIES
	 GENERATED TRUE )

aux_source_directory(${CMAKE_SOURCE_DIR}/src  SRC)


add_executable(${PROJECT_NAME} 
	${PROTO_SRC}
	${PROTO_HEADER}
	${SRC} 
	)

target_link_libraries(${PROJECT_NAME} 
		${PROTOBUF_LIBRAIES}
		)
