cmake_minimum_required( VERSION 3.10.0 )

#
# chech current working directory
#
if( CMAKE_BINARY_DIR STREQUAL CMAKE_CURRENT_LIST_DIR )
	message( FATAL_ERROR "DO NOT build in src directory.\n"
						 "You can build by following steps:\n"
						 "\tPROMPT> cd ../build\n"
						 "\tPROMPT> make" )
endif()

project( Plastics )

set( PLASTICS_OUTPUT_DIRECTORY_BASE ${CMAKE_BINARY_DIR}/../output )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PLASTICS_OUTPUT_DIRECTORY_BASE}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PLASTICS_OUTPUT_DIRECTORY_BASE}/bin )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PLASTICS_OUTPUT_DIRECTORY_BASE}/bin )

set( SUB_MODULES
	autolibs
)

foreach( module ${SUB_MODULES} )
	add_subdirectory( ${module} )
endforeach()

add_custom_target( Plastics ALL
	COMMENT "Build Plastics"
)

foreach( module ${SUB_MODULES} )
	add_dependencies( Plastics ${module}_shared ${module}_static )
endforeach()

