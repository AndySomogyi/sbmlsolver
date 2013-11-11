cmake_minimum_required(VERSION 2.8)
set(target rrp_add_noise)

include_directories(
../source
../../source
../../wrappers/C
${THIRD_PARTY_INSTALL_FOLDER}/include
)

set(sources
add_noise
add_noise_thread
auto_link
)

add_library(${target} SHARED ${sources})

add_definitions(
-DEXPORT_RR_PLUGIN
-DSTATIC_RR
-DSTATIC_RR_PLUGIN
-DSTATIC_PLUGINS_API
-DPOCO_STATIC
)

if(WIN32)
target_link_libraries (${target}
roadrunner-static
rrplugins_api-static
)
else() #UNIX
    # OSX flags:
    #"${CMAKE_SHARED_LINKER_FLAGS} -Wl,-no_implicit_dylibs -undefined error")
    # OSX seems to link fine without this flag
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
        rr-libstruct-static
        nleq-static
        sbml-static
        lapack
        blas
        f2c
        pthread
        rrc_api
        rrplugins_c_api
        )
endif()

if(${MSVC})
#set (CMAKE_SHARED_LINKER_FLAGS     "${CMAKE_SHARED_LINKER_FLAGS} \"/DEF:${CMAKE_CURRENT_SOURCE_DIR}/add_noise.def\"")
endif()

install (TARGETS ${target}
DESTINATION plugins
COMPONENT plugins
)

