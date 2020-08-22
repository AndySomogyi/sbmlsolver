cmake_minimum_required(VERSION 2.8)
set(target bistable)

add_executable(${target} 
main.cpp 
)

add_definitions(
-DSTATIC_RR
-DSTATIC_PLUGIN_API
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
roadrunner-static
telplugins_api-static
libxml2
libsbml-static
poco_foundation-static
rr-libstruct-static
sundials_cvode
sundials_nvecserial
nleq-static
blas
lapack
libf2c
)
endif()

install (TARGETS ${target} 		
        DESTINATION bin 
        COMPONENT examples)
