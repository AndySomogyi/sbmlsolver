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


# configure compilation of modules for old and new player. Old player modules are compiled with ("threads"=0) option and new are compiled with ("threads"=1) option
# set(thread_option \(\"threads\"=1\))






SET_SOURCE_FILES_PROPERTIES(sbmlsolver.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(sbmlsolver.i PROPERTIES SWIG_FLAGS "-threads")


if(USE_TR1_CXX_NS)
  message(STATUS "Using tr1 for SWIG smart pointer namespace")
  set(CMAKE_SWIG_FLAGS "-DSWIG_SHARED_PTR_SUBNAMESPACE=tr1")
endif()

SWIG_ADD_MODULE(sbmlsolver python sbmlsolver.i PyUtils PyLoggerStream)





if(WIN32)
    # changes dll name to pyd sop that it is compatible with new Python standard
    SET_TARGET_PROPERTIES(_sbmlsolver PROPERTIES SUFFIX ".pyd")


    set_property(TARGET _sbmlsolver
        PROPERTY  COMPILE_DEFINITIONS
        EXPORT_RRC
        LIBSBML_STATIC
        LIBLAX_STATIC
        STATIC_PUGI
        POCO_STATIC
        STATIC_LIBSTRUCT
        STATIC_RR
        )

    SWIG_LINK_LIBRARIES ( sbmlsolver
        sbmlsolver-static
        ${PYTHON_LIBRARIES}
        )


else(WIN32)

    #    set(CMAKE_SHARED_LINKER_FLAGS "-Wl, -z,defs --no-undefined")
    #    set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined")

    if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        # Linux specific code
        #SET_TARGET_PROPERTIES(_sbmlsolver PROPERTIES LINK_FLAGS "-Wl,--no-undefined")

    else()
        # OSX section.
        # we do not link directly with python at at link time, so that at run time,
        # the hosting python would provice all the sybols, we only
        # link with the external libs that we uses.
        SET_TARGET_PROPERTIES(_sbmlsolver PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
    endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

    set_property(TARGET _sbmlsolver
        PROPERTY  COMPILE_DEFINITIONS
        LIBSBML_STATIC
        LIBLAX_STATIC
        STATIC_PUGI
        STATIC_LIBSTRUCT
        )

    SWIG_LINK_LIBRARIES ( sbmlsolver
        sbmlsolver-static
        rr-libstruct-static
        nleq-static
        sbml-static
        lapack
        blas
        f2c
#        ${PYTHON_LIBRARIES}
        )

    #        lapack
    #        blas
    #        f2c
    #        "-Wl,--whole-archive"
    #        rr-libstruct-static
    #        "-Wl,--no-whole-archive"
    #        lmfit-static
    #        sbml-static
    #        sbmlsolver-static


endif(WIN32)

set(RR_PACKAGE_DIR site-packages/sbmlsolver)

install(
    TARGETS _sbmlsolver
    DESTINATION ${RR_PACKAGE_DIR}
)

set(python_files_path ${CMAKE_BINARY_DIR}/wrappers/Python/sbmlsolver/)

#THIS IS EXAMPLE OF WORKING COMMAND
# ADD_CUSTOM_COMMAND(
#     OUTPUT CompuCell.py.mod
#     COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/core/pyinterface/swigmodifier.py ${python_files_path}/CompuCell.py
#     DEPENDS _CompuCell
# )
#
# ADD_CUSTOM_TARGET(_swigmodifier ALL DEPENDS CompuCell.py.mod)

#FILE(GLOB Mac_CPP "*_Mac.cpp")

#INSTALL(FILES ${python_files_path}/sbmlsolver.py
#    DESTINATION site-packages/sbmlsolver )

install(
    FILES ${python_files_path}/sbmlsolver.py
    DESTINATION ${RR_PACKAGE_DIR}
)

install(
    FILES __init__.py
    DESTINATION ${RR_PACKAGE_DIR}
)

install(
    DIRECTORY testing
    DESTINATION ${RR_PACKAGE_DIR}
    FILES_MATCHING PATTERN "*.*"
)

install(
    DIRECTORY stochastic
    DESTINATION ${RR_PACKAGE_DIR}
    FILES_MATCHING PATTERN "*.*"
)

file(GLOB rrtest_files "${RR_ROOT}/testing/*.rrtest")
install(
    FILES ${rrtest_files} 
    DESTINATION ${RR_PACKAGE_DIR}/testing/test_data
)

