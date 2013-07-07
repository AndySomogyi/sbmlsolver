cmake_minimum_required(VERSION 2.8)
set(target add_noise)

include_directories(
../../source 
../../wrappers/C 
${THIRD_PARTY_INSTALL_FOLDER}/include
)

set(sources 
add_noise 
add_noise_thread
)

add_library(${target} SHARED ${sources})

add_definitions(
-DPOCO_STATIC
-DSTATIC_PUGI
-DLIBSBML_STATIC 
-DSTATIC_RR
-DSTATIC_LIBSTRUCT
-DLIBLAX_STATIC  
-DSTATIC_NLEQ
-DEXPORT_PLUGIN
)

if(WIN32)
add_definitions( -DWIN32)
target_link_libraries (${target} 
roadrunner-static
)
endif()

#Not sure why linux linker need all libraries at linktime?? All symbols 'should' be in roadrunner-static
if(UNIX)
target_link_libraries (${target} 
roadrunner-static
nleq-static.a
rr-libstruct-static
lapack
blas
f2c
sbml-static.a
xml2
sundials_nvecserial.a
sundials_cvode.a
pthread
poco_foundation-static.a
pugi-static.a
dl
)
endif()

if(${MSVC})
set (CMAKE_SHARED_LINKER_FLAGS 	"${CMAKE_SHARED_LINKER_FLAGS} \"/DEF:${CMAKE_CURRENT_SOURCE_DIR}/add_noise.def\"")
endif()

install (TARGETS ${target} 		
DESTINATION plugins 
COMPONENT plugins
)

#Skip installing any header
