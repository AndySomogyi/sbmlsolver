cmake_minimum_required(VERSION 2.8)
set(target rrp_lm)

include_directories(
../source
../../source
../../wrappers/C
${THIRD_PARTY_INSTALL_FOLDER}/include
${CMAKE_INSTALL_PREFIX}/include
)

link_directories(
${CMAKE_INSTALL_PREFIX}/lib
)

set(sources
lm
lm_thread
lmUtils

)

#This is where the Levenberg C lib is built
add_subdirectory(lib)

add_definitions(
-DPOCO_STATIC
-DLIBSBML_STATIC
-DSTATIC_RR
-DSTATIC_PLUGINS_API
-DEXPORT_RR_PLUGIN
)

add_library(${target} SHARED ${sources})

if(WIN32)
target_link_libraries (${target}
lmfit-static
rrplugins_api-static
rrc_api                 
#roadrunner-static
#poco_foundation-static
#blas
#lapack
#rr-libstruct-static
#libf2c
#sundials_cvode
#sundials_nvecserial
#nleq-static
)

else() 
    if(APPLE)
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-no_implicit_dylibs -undefined error")
    else()
        set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined")
    endif()

    set_property(TARGET ${target}
        PROPERTY  COMPILE_DEFINITIONS
        LIBSBML_STATIC
        LIBLAX_STATIC
        STATIC_PUGI
        STATIC_LIBSTRUCT
        )

    target_link_libraries ( ${target}
        roadrunner-static
        lmfit-static
        rrc_api
        rrplugins_api-static
        )
endif()

install (TARGETS ${target}
DESTINATION plugins
COMPONENT plugins
)
