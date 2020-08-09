cmake_minimum_required( VERSION 3.18.1 )

project( auto_libs )

set( AUTO_RESULT       auto_result )
set( AUTO_MEMORY       auto_memory )

set( SRC_FILES
	${AUTO_RESULT}/auto_result.cpp
	${AUTO_MEMORY}/auto_memory.cpp
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

add_library( auto_libs_shared SHARED ${SRC_FILES} )
set_target_properties( auto_libs_shared PROPERTIES PREFIX "" )
set_target_properties( auto_libs_shared PROPERTIES OUTPUT_NAME libauto )

add_library( auto_libs_static ${SRC_FILES} )
set_target_properties( auto_libs_static PROPERTIES PREFIX "" )
set_target_properties( auto_libs_static PROPERTIES OUTPUT_NAME libauto )

add_library( auto_libs_object ${SRC_FILES} )
set_target_properties( auto_libs_object PROPERTIES PREFIX "" )
set_target_properties( auto_libs_object PROPERTIES OUTPUT_NAME libauto_obj )

add_library( auto_libs_object_pic ${SRC_FILES} )
set_target_properties( auto_libs_object_pic PROPERTIES POSITION_INDEPENDENT_CODE ON )
set_target_properties( auto_libs_object_pic PROPERTIES PREFIX "" )
set_target_properties( auto_libs_object_pic PROPERTIES OUTPUT_NAME libauto_obj_pic )

