# copy to /lib so we can import roadrunner in the build tree
configure_file(__init__.py ${CMAKE_BINARY_DIR}/lib/site-packages/roadrunner/__init__.py COPYONLY)

set_source_files_properties(roadrunner.i PROPERTIES CPLUSPLUS ON)
set_source_files_properties(roadrunner.i PROPERTIES SWIG_FLAGS "-threads")

if (USE_TR1_CXX_NS)
    message(STATUS "Using tr1 for SWIG smart pointer namespace")
    set(CMAKE_SWIG_FLAGS "-DSWIG_SHARED_PTR_SUBNAMESPACE=tr1")
endif ()

swig_add_library(roadrunner_python
        TYPE SHARED LANGUAGE python
        SOURCES roadrunner.i PyUtils PyLoggerStream)


set_target_properties(roadrunner_python PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/python/roadrunner
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/site-packages/roadrunner
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/site-packages/roadrunner
        OUTPUT_NAME roadrunner
        SWIG_USE_TARGET_INCLUDE_DIRECTORIES TRUE # Needed for target_include_directories to have any effect on swig target
        )

target_include_directories(roadrunner_python PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
        ${RR_SOURCE_ROOT}
        "$<TARGET_PROPERTY:roadrunner-static,INCLUDE_DIRECTORIES>"
        ${RR_DEPENDENCIES_INSTALL_PREFIX}/include/rr-libstruct
        ${PYTHON_INCLUDE_PATH}
        ${NUMPY_INCLUDE_DIRS}
        )

if (WIN32)
    target_compile_definitions(roadrunner_python PRIVATE EXPORT_RRC STATIC_RR)

    swig_link_libraries(roadrunner_python
            roadrunner-static
            ${PYTHON_LIBRARIES}
            )
    # changes dll name to pyd sop that it is compatible with new Python standard
    set_target_properties(roadrunner_python PROPERTIES SUFFIX ".pyd")
elseif (UNIX AND NOT APPLE)
    # Linux specific code
    #SET_TARGET_PROPERTIES(roadrunner_python PROPERTIES LINK_FLAGS "-Wl,--no-undefined")
    swig_link_libraries(roadrunner_python
            roadrunner-static
            ${PYTHON_LIBRARIES}
            )
    # Note: elseif(APPLE) did not work
elseif (APPLE)
    # note: this section is old (i.e. not part of new build system) so I (cw) have not verified
    # that it is needed.
    # todo (cw) link against roadrunner-static and try to run some python tests. Does it work? If so
    #  then we do not need special case code for macos and the script simplifies.
    #
    #
    # OSX section.
    # we do not link directly with python at at link time, so that at run time,
    # the hosting python would provice all the symbols, we only
    # link with the external libs that we use.
    set_target_properties(roadrunner_python PROPERTIES
            LINK_FLAGS "-undefined dynamic_lookup"
            INSTALL_RPATH "@loader_path/../../lib;@loader_path/;@executable_path"
            INSTALL_NAME_DIR "@rpath"
            )
    # we use roadrunner not roadrunner-static for
    swig_link_libraries(roadrunner_python
            roadrunner
            ${PYTHON_LIBRARIES}
            )
else ()
    message(STATIC "system not supported")
endif ()

install(
        TARGETS roadrunner_python
        DESTINATION ${PYTHON_PACKAGE_INSTALL_PREFIX}
)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/roadrunner.py"
        DESTINATION "${PYTHON_PACKAGE_INSTALL_PREFIX}")


install(
        DIRECTORY testing
        DESTINATION ${PYTHON_PACKAGE_INSTALL_PREFIX}
        FILES_MATCHING PATTERN "*"
)

install(
        DIRECTORY stochastic
        DESTINATION ${PYTHON_PACKAGE_INSTALL_PREFIX}
        FILES_MATCHING PATTERN "*.*"
)

file(GLOB rrtest_files "${RR_ROOT}/testing/*.rrtest")
install(
        FILES ${rrtest_files}
        DESTINATION ${PYTHON_PACKAGE_INSTALL_PREFIX}/testing/test_data
)

file(GLOB rrtest_files "${RR_ROOT}/testing/*.xml")
install(
        FILES ${rrtest_files}
        DESTINATION ${PYTHON_PACKAGE_INSTALL_PREFIX}/testing/test_data
)
