cmake_minimum_required(VERSION 2.8)
set(target one_rr)

add_executable(${target} 
main.cpp 
)

add_definitions(
-DPOCO_STATIC
-DLIBSBML_STATIC 
-DSTATIC_LIBSTRUCT
-DLIBLAX_STATIC  
-DSTATIC_NLEQ
)

if(WIN32)
add_definitions(
-DWIN32
)

target_link_libraries (${target} 
roadrunner
telplugins_c_api
)
endif()

#Not sure why linux linker need all libraries at linktime?? All symbols 'should' be in roadrunner-static

install (TARGETS ${target} 		
        DESTINATION bin 
        COMPONENT examples)
