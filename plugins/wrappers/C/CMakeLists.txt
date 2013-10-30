set(target rrplugins_c_api)

include_directories(${RR_GENERATED_HEADER_PATH}) #we have to do this to get the generated header

include_directories(
${RR_ROOT}/source
${RR_ROOT}/plugins/source
${THIRD_PARTY_INSTALL_FOLDER}/include
)

#The C-api is built as a shared library..
add_library(${target} SHARED
rrp_api
rrp_cpp_support
rrp_minimization_api
rrp_parameter_api
)

if(${MSVC})
    set (CMAKE_SHARED_LINKER_FLAGS     "${CMAKE_SHARED_LINKER_FLAGS} \"/DEF:${CMAKE_CURRENT_SOURCE_DIR}/rrp_api.def\"")
endif()

#Link C_api dll statically to roadrunner and libsbml
if(WIN32)
    set_property(TARGET ${target}
        PROPERTY  COMPILE_DEFINITIONS
        EXPORT_RRP
        LIBSBML_STATIC
        LIBLAX_STATIC
        POCO_STATIC
        STATIC_LIBSTRUCT
        STATIC_RR
        STATIC_PLUGINS_API
        )

    target_link_libraries ( ${target}
        rrc_api
        roadrunner-static
        rrplugins_api-static
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
        rrc_api
        roadrunner-static
        rrPluginsAPI
        rr-libstruct-static
        nleq-static
        sbml-static
        lapack
        blas
        f2c
        pthread
        )
endif()

#add_subdirectory(Apps)
#if(INSTALL_EXAMPLES)
#    add_subdirectory(Examples)
#endif()
#
#if(BUILD_TESTS)
#    add_subdirectory(Testing)
#endif()
#
#=============== INSTALL =====================
install(
    FILES
    rrp_api.h
    rrp_api.h
    rrp_utilities.h
    rrp_types.h
    rrp_exporter.h

    DESTINATION include/plugins_api COMPONENT plugins_api
    )

install(
    TARGETS ${target}
    RUNTIME DESTINATION bin COMPONENT plugins_api
    ARCHIVE DESTINATION lib COMPONENT plugins_api
    LIBRARY DESTINATION lib COMPONENT plugins_api
    )

