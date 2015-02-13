#we have to do this to get the generated header
include_directories(${RR_GENERATED_HEADER_PATH})

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${RR_SOURCE_ROOT}
    ${THIRD_PARTY_INSTALL_FOLDER}/include
    ${THIRD_PARTY_INSTALL_FOLDER}/include/rr-libstruct
    ${THIRD_PARTY_INSTALL_FOLDER}/include/sbml
    ${THIRD_PARTY_INSTALL_FOLDER}/include/cvode
    )

set_source_files_properties(jroadrunner.i PROPERTIES CPLUSPLUS ON)

swig_add_module(jroadrunner java jroadrunner.i)

if(WIN32)
    message(FATAL_ERROR "Java build on Windows not supported")
    # changes dll name to pyd sop that it is compatible with new Python standard
#     set_property(TARGET jroadrunner
#         PROPERTY  COMPILE_DEFINITIONS
#         EXPORT_RRC
#         LIBSBML_STATIC
#         LIBLAX_STATIC
#         STATIC_PUGI
#         POCO_STATIC
#         STATIC_LIBSTRUCT
#         STATIC_RR
#         )
#
#     swig_link_libraries ( jroadrunner
#         jroadrunner-static
#         ${Java_LIBRARIES}
#         )
else(WIN32)
    #    set(CMAKE_SHARED_LINKER_FLAGS "-Wl, -z,defs --no-undefined")
    #    set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined")

    if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        # Linux specific code
        #SET_TARGET_PROPERTIES(_jroadrunner PROPERTIES LINK_FLAGS "-Wl,--no-undefined")
    else()
#         SET_TARGET_PROPERTIES(_jroadrunner PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
    endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

    set_property(TARGET jroadrunner
        PROPERTY  COMPILE_DEFINITIONS
        LIBSBML_STATIC
        LIBLAX_STATIC
        STATIC_PUGI
        STATIC_LIBSTRUCT
        )

    swig_link_libraries ( jroadrunner
        roadrunner-static
        rr-libstruct-static
        nleq-static
        sbml-static
        lapack
        blas
        f2c
       ${Java_LIBRARIES}
        )

    #        lapack
    #        blas
    #        f2c
    #        "-Wl,--whole-archive"
    #        rr-libstruct-static
    #        "-Wl,--no-whole-archive"
    #        lmfit-static
    #        sbml-static
    #        jroadrunner-static


endif(WIN32)
