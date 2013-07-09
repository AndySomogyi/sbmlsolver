cmake_minimum_required(VERSION 2.8)
set(target lm)

include_directories(
../../source 
../../wrappers/C 
${THIRD_PARTY_INSTALL_FOLDER}/include
)

set(sources 
lm 
lm_thread
lmUtils
)


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

add_library(${target} SHARED ${sources})

if(WIN32)
add_definitions( -DWIN32)
target_link_libraries (${target} 
lmfit-static
rrc_api
)
endif()

#Not sure why linux linker need all libraries at linktime?? All symbols 'should' be in roadrunner-static
if(UNIX)
target_link_libraries (${target} 
lmfit-static
rrc_api
)
endif()

if(${MSVC})
set (CMAKE_SHARED_LINKER_FLAGS 	"${CMAKE_SHARED_LINKER_FLAGS} \"/DEF:${CMAKE_CURRENT_SOURCE_DIR}/${target}.def\"")
endif()

install (TARGETS ${target} 		
DESTINATION plugins 
COMPONENT plugins
)

#Skip installing any header
