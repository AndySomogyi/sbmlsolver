cmake_minimum_required(VERSION 2.8)

set(target validate_sbml)

add_executable(${target} 
main.cpp 
Args.cpp
)

#link with shared roadrunner
target_link_libraries (${target} roadrunner)
#target_link_libraries (${target} libsbml)
#

install (   TARGETS ${target} 		
            DESTINATION bin 
            COMPONENT compiled_examples)

set_property(DIRECTORY PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${exe_path}/${target}.tds)
