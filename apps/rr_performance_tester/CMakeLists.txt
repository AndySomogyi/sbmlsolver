cmake_minimum_required(VERSION 2.8)
set(target rr_performance_tester)

add_executable( ${target} 
main.cpp 
Args.cpp
)

target_link_libraries (${target} 
roadrunner
libsbml
poco_foundation-static
)

install (TARGETS ${target} 		
        DESTINATION bin 
        COMPONENT apps)
