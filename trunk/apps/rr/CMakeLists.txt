cmake_minimum_required(VERSION 2.8)
set(target rr)

add_executable(		   ${target} 
main.cpp 
Args.cpp
)

#IF(BUILD_SHARED_LIBS) 
target_link_libraries (${target} roadrunner)
target_link_libraries (${target} libsbml)
#ELSE()
#target_link_libraries (${target} -static roadrunner-static)
#target_link_libraries (${target} -static libsbml-static)
#ENDIF()


##target_link_libraries (${target} libxml2_xe)
#target_link_libraries (${target} sundials_cvode)
#target_link_libraries (${target} sundials_nvecserial)
#target_link_libraries (${target} lapack)
#target_link_libraries (${target} blas)
#target_link_libraries (${target} libf2c)


install (TARGETS ${target} 		
        DESTINATION bin 
        COMPONENT apps)
