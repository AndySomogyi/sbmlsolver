project(threads_2 C)
set(target threads_2)

add_executable( ${target} 
main.c
)

target_link_libraries (${target} rr_c_api)

install(
TARGETS ${target} 		
DESTINATION bin 
COMPONENT compiled_examples
)
