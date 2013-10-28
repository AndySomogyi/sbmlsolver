CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
set(target rrPluginsAPI)

include_directories(${RR_GENERATED_HEADER_PATH}) #we have to do this to get the generated header

include_directories(
${RR_ROOT}/source
${THIRD_PARTY_INSTALL_FOLDER}/include
)

add_library(${target} SHARED
    rrPlugin
    rrPluginManager
#    rrCPPPlugin
    rrCPlugin
    rrCapability
    rrCapabilities
    rrBaseParameter
    rrParameters
    rrMinimizationData
)

#Link plugin api dll statically to roadrunner and libsbml
if(WIN32)
    set_property(TARGET ${target}
        PROPERTY  COMPILE_DEFINITIONS
        EXPORT_RRP
        LIBSBML_STATIC
        LIBLAX_STATIC
        STATIC_PUGI
        POCO_STATIC
        STATIC_LIBSTRUCT
        STATIC_RR
        )

    target_link_libraries ( ${target}
        roadrunner-static
        )
else() #Unix
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
        )
endif()


#=============== INSTALL =====================
#install(
#    FILES
#    ${target}.h
#    rrc_utilities.h
#    rrc_types.h
#    rrc_exporter.h
#
#    DESTINATION include/c_api COMPONENT c_api
#    )

install(
    TARGETS ${target}
    RUNTIME DESTINATION bin COMPONENT plugins_api
    ARCHIVE DESTINATION lib COMPONENT plugins_api
    LIBRARY DESTINATION lib COMPONENT plugins_api
    )

