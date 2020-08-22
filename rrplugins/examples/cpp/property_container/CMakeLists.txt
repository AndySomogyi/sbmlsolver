cmake_minimum_required(VERSION 2.8)
set(target property_meta_data)

include_directories(
${THIRD_PARTY_INSTALL_FOLDER}/include
)

set(sources
main
)

add_executable(${target} ${sources})

add_definitions(
-DPOCO_STATIC
)

if(WIN32)
target_link_libraries (${target}
roadrunner
telplugins
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
        rr-libstruct-static
        nleq-static
        sbml-static
        lapack
        blas
        f2c
        pthread
        #roadrunner_c_api
        telplugins_c_api
        )
endif()

install (TARGETS ${target}
DESTINATION bin
COMPONENT examples
)

