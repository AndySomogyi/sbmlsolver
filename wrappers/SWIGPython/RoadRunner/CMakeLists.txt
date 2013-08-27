# SET(NUMPY_INCLUDE_DIR )
# EXEC_PROGRAM(${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/core/pyinterface/PlayerPythonNew ARGS NumpyHeader.py
# OUTPUT_VARIABLE NUMPY_INCLUDE_DIR)


#we have to do this to get the generated header
include_directories(${RR_GENERATED_HEADER_PATH})

include_directories(
    ${RR_ROOT}/source
    ${THIRD_PARTY_INSTALL_FOLDER}/include
)


INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${RR_SOURCE_ROOT}
    ${THIRD_PARTY_INSTALL_FOLDER}/include
    ${THIRD_PARTY_INSTALL_FOLDER}/include/rr-libstruct
    ${THIRD_PARTY_INSTALL_FOLDER}/include/sbml
    ${THIRD_PARTY_INSTALL_FOLDER}/include/cvode
    # ${RR_ROOT}
    # ${THIRD_PARTY_FOLDER}/sundials/include
    )


# configure compilation of modules for old and new player. Old player modules are compiled with ("threads"=0) option and new are compiled with ("threads"=1) option
# set(thread_option \(\"threads\"=1\))



SET_SOURCE_FILES_PROPERTIES(RoadRunner.i  PROPERTIES CPLUSPLUS ON)
# SET_SOURCE_FILES_PROPERTIES(CompuCellPython.i CompuCellAuxPython.i PROPERTIES SWIG_FLAGS "-includeall")
SWIG_ADD_MODULE(RoadRunner python RoadRunner.i)



if(WIN32)
    # changes dll name to pyd sop that it is compatible with new Python standard
    SET_TARGET_PROPERTIES(_RoadRunner PROPERTIES SUFFIX ".pyd")


    set_property(TARGET _RoadRunner
        PROPERTY  COMPILE_DEFINITIONS
        EXPORT_RRC
        LIBSBML_STATIC
        LIBLAX_STATIC
        STATIC_PUGI
        POCO_STATIC
        STATIC_LIBSTRUCT
        STATIC_RR
        )

    SWIG_LINK_LIBRARIES ( RoadRunner
        roadrunner-static
        ${PYTHON_LIBRARIES}
        )


else(WIN32)

#    set(CMAKE_SHARED_LINKER_FLAGS "-Wl, -z,defs --no-undefined")
#    set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined")

    if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        # Linux specific code
        SET_TARGET_PROPERTIES(_RoadRunner PROPERTIES LINK_FLAGS "-Wl,--no-undefined")
    endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")



    set_property(TARGET _RoadRunner
        PROPERTY  COMPILE_DEFINITIONS
        LIBSBML_STATIC
        LIBLAX_STATIC
        STATIC_PUGI
        STATIC_LIBSTRUCT
        )

    SWIG_LINK_LIBRARIES ( RoadRunner
        roadrunner-static
        rr-libstruct-static
        nleq-static
        sbml-static
        lapack
        blas
        f2c
        ${PYTHON_LIBRARIES}
        )

        #        lapack
        #        blas
        #        f2c
        #        "-Wl,--whole-archive"
        #        rr-libstruct-static
        #        "-Wl,--no-whole-archive"
        #        lmfit-static
        #        sbml-static
        #        roadrunner-static



endif(WIN32)





install_targets(/lib/python _RoadRunner)

set(python_files_path ${CMAKE_BINARY_DIR}/wrappers/SWIGPython/RoadRunner/)

#THIS IS EXAMPLE OF WORKING COMMAND
# ADD_CUSTOM_COMMAND(
#     OUTPUT CompuCell.py.mod
#     COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/core/pyinterface/swigmodifier.py ${python_files_path}/CompuCell.py
#     DEPENDS _CompuCell
# )
#
# ADD_CUSTOM_TARGET(_swigmodifier ALL DEPENDS CompuCell.py.mod)

INSTALL(FILES ${python_files_path}/RoadRunner.py
    DESTINATION lib/python)


