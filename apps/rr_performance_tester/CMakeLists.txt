cmake_minimum_required(VERSION 2.8)
set(target rr_performance_tester)

add_executable( ${target} 
main.cpp 
Args.cpp
)

target_link_libraries (${target} roadrunner)
target_link_libraries (${target} libsbml)

install (TARGETS ${target} 		
        DESTINATION bin 
        COMPONENT apps)
