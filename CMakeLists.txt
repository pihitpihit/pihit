cmake_minimum_required( VERSION 3.10.0 )

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

if( WIN32 )
	set( PLS_COPY xcopy /E /Y )
else()
	set( PLS_COPY cp -r )
endif()

foreach( module ${SUB_MODULES} )
	file( TO_NATIVE_PATH ${CMAKE_CURRENT_LIST_DIR}/${module}/${OUTPUT_DIR}/* module_output_dir )
	file( TO_NATIVE_PATH ${CMAKE_CURRENT_LIST_DIR}/${OUTPUT_DIR} output_dir )
	add_custom_command( TARGET Plastics POST_BUILD COMMAND echo ${PLS_COPY} ${module_output_dir} ${output_dir} )
	add_custom_command( TARGET Plastics POST_BUILD COMMAND ${PLS_COPY} ${module_output_dir} ${output_dir} )
endforeach()


