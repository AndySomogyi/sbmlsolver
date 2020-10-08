# copy to /lib so we can import roadrunner in the build tree
configure_file(__init__.py ${CMAKE_BINARY_DIR}/lib/site-packages/roadrunner/__init__.py COPYONLY)


set_source_files_properties(roadrunner.i PROPERTIES CPLUSPLUS ON)
set_source_files_properties(roadrunner.i PROPERTIES SWIG_FLAGS "-threads")

if (USE_TR1_CXX_NS)
    message(STATUS "Using tr1 for SWIG smart pointer namespace")
    set(CMAKE_SWIG_FLAGS "-DSWIG_SHARED_PTR_SUBNAMESPACE=tr1")
endif ()

# todo replace this with swig_add_library as swig add module is deprecated.
#swig_add_module(roadrunner_python python roadrunner.i PyUtils PyLoggerStream)
swig_add_library(roadrunner_python
        TYPE SHARED LANGUAGE python
        SOURCES roadrunner.i PyUtils PyLoggerStream)


set_target_properties(roadrunner_python PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/python/roadrunner
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/site-packages/roadrunner
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/site-packages/roadrunner
        OUTPUT_NAME roadrunner
        SWIG_USE_TARGET_INCLUDE_DIRECTORIES TRUE # Needed for target_include_directories to have any effect
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
    # changes dll name to pyd sop that it is compatible with new Python standard
    set_target_properties(roadrunner_python PROPERTIES SUFFIX ".pyd")

    target_compile_definitions(roadrunner_python PRIVATE EXPORT_RRC STATIC_RR)
    swig_link_libraries(roadrunner_python
            roadrunner-static
            ${PYTHON_LIBRARIES}
            )


elseif (UNIX)

    #    set(CMAKE_SHARED_LINKER_FLAGS "-Wl, -z,defs --no-undefined")
    #    set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined")

    # Linux specific code
    #SET_TARGET_PROPERTIES(roadrunner_python PROPERTIES LINK_FLAGS "-Wl,--no-undefined")

elseif (MACOS)
    # OSX section.
    # we do not link directly with python at at link time, so that at run time,
    # the hosting python would provice all the sybols, we only
    # link with the external libs that we uses.
    set_target_properties(roadrunner_python PROPERTIES LINK_FLAGS "-undefined dynamic_lookup" INSTALL_RPATH "@loader_path/../../lib;@loader_path/" INSTALL_NAME_DIR "@rpath")
    swig_link_libraries(roadrunner_python
            roadrunner-static
            #        ${PYTHON_LIBRARIES}
            )
else ()
    message(STATIC "system not supported")
endif ()

install(
        TARGETS roadrunner_python
        DESTINATION ${PYTHON_PACKAGE_DEST_DIR}
)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/roadrunner.py"
    DESTINATION "${PYTHON_PACKAGE_DEST_DIR}" )



install(
        DIRECTORY testing
        DESTINATION ${PYTHON_PACKAGE_DEST_DIR}
        FILES_MATCHING PATTERN "*"
)

install(
        DIRECTORY stochastic
        DESTINATION ${PYTHON_PACKAGE_DEST_DIR}
        FILES_MATCHING PATTERN "*.*"
)

file(GLOB rrtest_files "${RR_ROOT}/testing/*.rrtest")
install(
        FILES ${rrtest_files}
        DESTINATION ${PYTHON_PACKAGE_DEST_DIR}/testing/test_data
)

file(GLOB rrtest_files "${RR_ROOT}/testing/*.xml")
install(
        FILES ${rrtest_files}
        DESTINATION ${PYTHON_PACKAGE_DEST_DIR}/testing/test_data
)
