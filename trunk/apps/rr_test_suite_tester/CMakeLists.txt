cmake_minimum_required(VERSION 2.8)
set(target rr_ts_tester)

add_executable(		   ${target} 
main.cpp 
Args.cpp
)

#IF(BUILD_SHARED_LIBS) 
#target_link_libraries (${target} rr)
#target_link_libraries (${target} libsbml)
#ELSE()
target_link_libraries (${target} roadrunner)
target_link_libraries (${target} libsbml)
#ENDIF()

set_property(DIRECTORY PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${exe_path}/${target}.tds)

install (TARGETS ${target} 		
        DESTINATION bin 
        COMPONENT apps)

install (   FILES rrExcludedTestCases.txt	
            DESTINATION bin 
            COMPONENT apps)
