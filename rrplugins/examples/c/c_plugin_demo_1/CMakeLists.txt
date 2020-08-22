cmake_minimum_required(VERSION 2.8)
project(tel_c_plugin_demo C)
ENABLE_LANGUAGE (C)
set(target tel_c_plugin_demo)

include_directories(
${TLP_ROOT}/source/core
${TLP_ROOT}/source/common
${TLP_ROOT}/wrappers/C
${THIRD_PARTY_INSTALL_FOLDER}/include
${THIRD_PARTY_INSTALL_FOLDER}/include/c_api
)

SET_SOURCE_FILES_PROPERTIES( c_plugin_demo.c PROPERTIES LANGUAGE C)
set(sources
c_plugin_demo.c
c_plugin_demo.h
)

add_library(${target} MODULE ${sources})

add_definitions(
-DEXPORT_TEL_PLUGIN
-DPOCO_STATIC
)


if(WIN32)
target_link_libraries (${target}
roadrunner
telplugins_c_api
roadrunner_c_api
)
#else() #UNIX
#    # OSX flags:
#    #"${CMAKE_SHARED_LINKER_FLAGS} -Wl,-no_implicit_dylibs -undefined error")
#    # OSX seems to link fine without this flag
#    if(APPLE)
#        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-no_implicit_dylibs -undefined error")
#    else()
#        set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined")
#    endif()
#
#    set_property(TARGET ${target}
#        PROPERTY  COMPILE_DEFINITIONS
#        LIBSBML_STATIC
#        LIBLAX_STATIC
#        STATIC_LIBSTRUCT
#        )
#
#    target_link_libraries ( ${target}
#        roadrunner-static
#        rr-libstruct-static
#        nleq-static
#        sbml-static
#        lapack
#        blas
#        f2c
#        pthread
#        roadrunner_c_api
#        telplugins_c_api
#        )
endif()

#Also build the main executable
set(main_target c_plugin_demo)

include_directories(
${TLP_ROOT}/source
${TLP_ROOT}/wrappers/C
${THIRD_PARTY_INSTALL_FOLDER}/include
)

set(main_sources
main.cpp
)

add_executable(${main_target} ${main_sources})

add_definitions(
-DPOCO_STATIC
)


if(WIN32)
target_link_libraries (${main_target}
roadrunner
roadrunner_c_api
telplugins_c_api
)
endif()

install (TARGETS ${target}
DESTINATION plugins
COMPONENT examples
)

install (TARGETS ${main_target}
DESTINATION bin
COMPONENT examples
)

