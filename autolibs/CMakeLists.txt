cmake_minimum_required( VERSION 3.18.1 )

find_program( CCACHE_PROGRAM ccache )
if( CCACHE_PROGRAM )
	set_property( GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}" )
endif()

project( auto_libs )

set( OUTPUT_DIR        output                )
set( OUTPUT_BIN        ${OUTPUT_DIR}/bin     )
set( OUTPUT_INC        ${OUTPUT_DIR}/include )
set( AUTO_RESULT       auto_result           )
set( AUTO_MEMORY       auto_memory           )

set( SRC_FILES
	${AUTO_RESULT}/auto_result.cpp
	${AUTO_MEMORY}/auto_memory.cpp
)

set( INC_FILES
	${AUTO_RESULT}/auto_result.h
	${AUTO_RESULT}/auto_result_list.h
	${AUTO_RESULT}/auto_result_maker.h
	${AUTO_MEMORY}/auto_memory.h
)

set( CMAKE_CXX_STANDARD 14 )

set(CMAKE_C_FLAGS "-Os -Wall")
set(CMAKE_CXX_FLAGS "-Os -Wall")

set(CMAKE_C_FLAGS_DEBUG "-Os -Wall -g3")
set(CMAKE_CXX_FLAGS_DEBUG  "-Os -Wall -g3")

set(CMAKE_C_FLAGS_RELEASE "-Os -Wall -DNDEBUG -s")
set(CMAKE_CXX_FLAGS_RELEASE "-Os -Wall -DNDEBUG -s")

include_directories(
	${AUTO_RESULT}
	${AUTO_MEMORY}
)


file( MAKE_DIRECTORY ${OUTPUT_DIR} )
file( MAKE_DIRECTORY ${OUTPUT_BIN} )
file( MAKE_DIRECTORY ${OUTPUT_INC} )

add_library( auto_libs_shared SHARED ${SRC_FILES} )
set_target_properties( auto_libs_shared PROPERTIES PREFIX "" )
set_target_properties( auto_libs_shared PROPERTIES OUTPUT_NAME ${OUTPUT_BIN}/libauto )

foreach( header ${INC_FILES} )
	add_custom_command( TARGET auto_libs_shared POST_BUILD COMMAND echo cp ${header} ${OUTPUT_INC} )
	add_custom_command( TARGET auto_libs_shared POST_BUILD COMMAND cp ${header} ${OUTPUT_INC} )
endforeach()

add_library( auto_libs_static STATIC ${SRC_FILES} )
set_target_properties( auto_libs_static PROPERTIES PREFIX "" )
set_target_properties( auto_libs_static PROPERTIES OUTPUT_NAME ${OUTPUT_BIN}/libauto )

add_library( auto_libs_object OBJECT ${SRC_FILES} )
set_target_properties( auto_libs_object PROPERTIES PREFIX "" )
set_target_properties( auto_libs_object PROPERTIES OUTPUT_NAME ${OUTPUT_BIN}/libauto_obj )

add_library( auto_libs_object_pic OBJECT ${SRC_FILES} )
set_target_properties( auto_libs_object_pic PROPERTIES POSITION_INDEPENDENT_CODE ON )
set_target_properties( auto_libs_object_pic PROPERTIES PREFIX "" )
set_target_properties( auto_libs_object_pic PROPERTIES OUTPUT_NAME ${OUTPUT_BIN}/libauto_obj_pic )

