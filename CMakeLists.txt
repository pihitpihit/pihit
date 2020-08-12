cmake_minimum_required( VERSION 3.16.3 )

project( Plastics )

set( OUTPUT_DIR output )

set( SUB_MODULES
	autolibs
)

file( MAKE_DIRECTORY ${OUTPUT_DIR} )

foreach( module ${SUB_MODULES} )
	add_subdirectory( ${module} )
endforeach()

add_custom_target( Plastics ALL
	COMMENT "Build Plastics"
)

foreach( module ${SUB_MODULES} )
	add_dependencies( Plastics ${module}_shared ${module}_static ${module}_object ${module}_object_pic )
endforeach()

foreach( module ${SUB_MODULES} )
	add_custom_command( TARGET Plastics POST_BUILD COMMAND echo cp -r ${module}/${OUTPUT_DIR}/* ${OUTPUT_DIR} )
	add_custom_command( TARGET Plastics POST_BUILD COMMAND cp -r ${module}/${OUTPUT_DIR}/* ${OUTPUT_DIR} )
endforeach()


