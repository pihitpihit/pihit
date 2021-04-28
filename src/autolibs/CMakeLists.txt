cmake_minimum_required( VERSION 3.10.0 )

find_program( CCACHE_PROGRAM ccache )
if( CCACHE_PROGRAM )
	set_property( GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}" )
endif()

project( autolibs )

set( AUTO_RESULT       result                )
set( AUTO_MEMORY       memory                )
set( AUTO_COMMON       common                )

file( TO_NATIVE_PATH ${PLASTICS_OUTPUT_DIRECTORY_BASE}/include  OUTPUT_INC  )
file( TO_NATIVE_PATH ${PLASTICS_OUTPUT_DIRECTORY_BASE}/test     OUTPUT_TEST )

# for test binaries
set( SUB_DIRS
	${AUTO_COMMON}
	${AUTO_RESULT}
	${AUTO_MEMORY}
)

set( SRC_FILES
	${AUTO_COMMON}/auto_prime.cpp
	${AUTO_COMMON}/auto_thread.cpp
	${AUTO_COMMON}/auto_log.cpp
	${AUTO_COMMON}/auto_time.cpp
	${AUTO_COMMON}/auto_sync.cpp
	${AUTO_RESULT}/auto_result.cpp
	${AUTO_MEMORY}/auto_memory.cpp
)

set( INC_FILES
	${AUTO_COMMON}/auto_prime.h
	${AUTO_COMMON}/auto_tls.hpp
	${AUTO_COMMON}/auto_thread.h
	${AUTO_COMMON}/auto_log.h
	${AUTO_COMMON}/auto_time.h
	${AUTO_COMMON}/auto_sync.h
	${AUTO_RESULT}/auto_result.h
	${AUTO_RESULT}/auto_result_list.h
	${AUTO_RESULT}/auto_result_maker.h
	${AUTO_MEMORY}/auto_memory.h
)

if( WIN32 )
	add_definitions( -DPLS_OS_WIN )
endif()

set( CMAKE_CXX_STANDARD 14 )

set(CMAKE_C_FLAGS "-Os -Wall")
set(CMAKE_CXX_FLAGS "-Os -Wall")

set(CMAKE_C_FLAGS_DEBUG "-Os -Wall ${FLAGS_G3}")
set(CMAKE_CXX_FLAGS_DEBUG  "-Os -Wall ${FLAGS_G3}")

set(CMAKE_C_FLAGS_RELEASE "-Os -Wall -DNDEBUG -s")
set(CMAKE_CXX_FLAGS_RELEASE "-Os -Wall -DNDEBUG -s")

include_directories( ${SUB_DIRS} )

file( MAKE_DIRECTORY ${OUTPUT_INC}  )
file( MAKE_DIRECTORY ${OUTPUT_TEST} )

add_library( autolibs_shared SHARED $<TARGET_OBJECTS:autolibs_object> )
set_target_properties( autolibs_shared PROPERTIES OUTPUT_NAME auto )

add_library( autolibs_static STATIC $<TARGET_OBJECTS:autolibs_object> )
set_target_properties( autolibs_static PROPERTIES OUTPUT_NAME auto_static )

add_library( autolibs_object OBJECT ${SRC_FILES} )
set_target_properties( autolibs_object PROPERTIES POSITION_INDEPENDENT_CODE ON )
set_target_properties( autolibs_object PROPERTIES OUTPUT_NAME auto_obj )

foreach( header ${INC_FILES} )
	file( TO_NATIVE_PATH ${CMAKE_CURRENT_LIST_DIR}/${header} header )
	add_custom_command( TARGET autolibs_shared POST_BUILD
		COMMAND cd
		COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_INC}
		COMMAND ${CMAKE_COMMAND} -E copy ${header} ${OUTPUT_INC} )
endforeach()

foreach( sub ${SUB_DIRS} )
	add_executable( test_${sub} ${sub}/main.cpp )
	set_target_properties( test_${sub}
		PROPERTIES OUTPUT_NAME test_${sub}
		RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_TEST} )
	target_link_libraries( test_${sub} PRIVATE autolibs_static )
endforeach()

