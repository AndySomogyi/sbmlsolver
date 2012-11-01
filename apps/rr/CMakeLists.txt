cmake_minimum_required(VERSION 2.8)
set(target rr)

add_executable(		   ${target} 
main.cpp 
Args.cpp
)

add_definitions(
-DSTATIC_POCO
#-DPOCO_NO_AUTOMATIC_LIBS
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
endif()


target_link_libraries (${target} 
roadrunner-static
nleq-static.a
rr-libstruct-static
lapack
blas
f2c
libsbml-static.a
libxml2.so
#z
sundials_nvecserial.a
sundials_cvode.a
pthread
poco_foundation-static.a
pugi-static.a
dl
)

install (TARGETS ${target} 		
        DESTINATION bin 
        COMPONENT apps)
