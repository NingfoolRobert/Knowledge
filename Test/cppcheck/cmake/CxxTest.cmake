
include("${CMAKE_CURRENT_LIST_DIR}/FindCxxTest.cmake")

ADD_DEFINITIONS(-DUNITTEST)

function(cxx_test target source)
    get_filename_component(CPP_FILE_NAME ${source} NAME)
    string(REGEX REPLACE "h$|hpp$" "cpp" CPP_FILE_NAME ${CPP_FILE_NAME})
    # message(${CPP_FILE_NAME})
    set(CPP_FULL_NAME "${CMAKE_CURRENT_LIST_DIR}/${CPP_FILE_NAME}")
    set(EXTERNALS_DIR "${TOP_DIR}/tools/")
    add_custom_command(
        OUTPUT "${CPP_FULL_NAME}"
        COMMAND ${CXXTESTGEN} --runner=ErrorPrinter --output "${CPP_FULL_NAME}" "${source}"
        DEPENDS "${source}"
    )
    add_executable(${target} ${CPP_FULL_NAME} ${EXTERNALS_DIR}/date/zdate.cpp)
    set_target_properties(${target} PROPERTIES COMPILE_FLAGS "-Wno-effc++")
    add_test(${target} ${RUNTIME_OUTPUT_DIRECTORY}/${target})
    target_include_directories(${target}
     PUBLIC                         
		"${EXTERNALS_DIR}/cxxtest/"
		"${TOP_DIR}/zslib/include"
		"${TOP_DIR}"
		"${TOP_DIR}/net"
		"${TOP_DIR}/net/protoc"
		"${TOP_DIR}/net/protoc/gprotoc"
		"${TOP_DIR}/net/include"
		"${TOP_DIR}/game/gs"
		"${TOP_DIR}/game"
		#"${TOP_DIR}/data/ElementData/Common"
		"${TOP_DIR}/tools"
		"${TOP_DIR}/tlog"
		"${TOP_DIR}/data-x"
		"${EXTERNALS_DIR}/date"
		"${EXTERNALS_DIR}/lua/include"
		"${EXTERNALS_DIR}/behavior/inc"
		"${EXTERNALS_DIR}/behavior/inc/behaviac"
		"${EXTERNALS_DIR}/luawrapper"
		"${TOP_DIR}/skill/skill"
		"${TOP_DIR}/unittest/include"
      )
endfunction(cxx_test)
