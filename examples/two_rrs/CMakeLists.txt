cmake_minimum_required(VERSION 2.8)
set(target many_threads)

add_executable(		   ${target} 
main.cpp 
)

add_definitions(
-DPOCO_STATIC
-DSTATIC_PUGI
-DLIBSBML_STATIC 
-DSTATIC_RR
-DSTATIC_LIBSTRUCT
-DLIBLAX_STATIC  
-DSTATIC_NLEQ
)

if(WIN32)
add_definitions(
-DWIN32
)

target_link_libraries (${target} 
sbmlsolver-static
)
endif()

#Not sure why linux linker need all libraries at linktime?? All symbols 'should' be in sbmlsolver-static
if(UNIX)
target_link_libraries (${target} 
sbmlsolver-static
nleq-static.a
rr-libstruct-static
lapack
blas
f2c
libsbml-static.a
libxml2.so
sundials_nvecserial.a
sundials_cvode.a
pthread
dl
)
endif()


install (TARGETS ${target} 		
        DESTINATION bin 
        COMPONENT examples)
