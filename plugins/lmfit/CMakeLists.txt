cmake_minimum_required(VERSION 2.8)
set(target lm)

include_directories(
../source
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
rrp_api
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
        lmfit-static
        rrc_api
        rrp_api
        )
endif()

if(${MSVC})
set (CMAKE_SHARED_LINKER_FLAGS     "${CMAKE_SHARED_LINKER_FLAGS} \"/DEF:${CMAKE_CURRENT_SOURCE_DIR}/${target}.def\"")
endif()

install (TARGETS ${target}
DESTINATION plugins
COMPONENT plugins
)

#Skip installing any header
