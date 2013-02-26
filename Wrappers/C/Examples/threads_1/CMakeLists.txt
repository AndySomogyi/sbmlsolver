set(target threads_1)

add_executable( ${target} 
main.c
)

set_source_files_properties(main.c PROPERTIES LANGUAGE C)
target_link_libraries (${target} rr_c_api)

install(
TARGETS ${target} 		
DESTINATION bin 
COMPONENT compiled_examples
)
