cmake_minimum_required(VERSION 2.8)
set(target rr_c)
set(rrSrc "../../../../source")

add_executable(${target} 
main.cpp 
Args.cpp
GetOptions.cpp
${rrSrc}/rrUtils.cpp
)

add_definitions(
-DSTATIC_POCO
-DSTATIC_PUGI
-DLIBSBML_STATIC
-DSTATIC_RR
-DSTATIC_LIBSTRUCT
-DLIBLAX_STATIC
-DSTATIC_NLEQ
)

target_link_libraries (${target} 
rr_c_api
)

#Not sure why linux linker need all libraries at linktime?? All symbols 'should' be in roadrunner-static
if(UNIX)
target_link_libraries (${target} 
#roadrunner-static
#nleq-static.a
#rr-libstruct-static
f2c
lapack
blas
#z
#libsbml-static.a
dl
z
xml2
#sundials_nvecserial.a
#sundials_cvode.a
pthread
#poco_foundation-static.a
#pugi-static.a
lzma_r
)
endif()

install(    
            TARGETS ${target} 		
            DESTINATION bin 
            COMPONENT apps)
