# "CMAKE_CURRENT_SOURCE_DIR" changes in the different files were
# add_test_executable gets called. We can use this as a constant
set(ThisDirectory ${CMAKE_CURRENT_SOURCE_DIR})

# Adds a new executable cmake target to cmake.
# Links roadrunner-static and gtest.
# Parameters:
#   - BINARY: name of the binary
#   - OUT_VARIABLE: list variable to append BINARY. Creates the list if not exist.
#   - All further arguments are added to the binary as sources.
function(add_test_executable BINARY OUT_VARIABLE)
    add_executable(${BINARY} ${ARGN})

    target_include_directories(
            ${BINARY} PRIVATE
            "${ThisDirectory}"
    )

    target_link_libraries(${BINARY} PUBLIC
            roadrunner-static gtest gtest_main gmock gmock_main

            )

    add_dependencies(${BINARY} roadrunner-static gtest gtest_main gmock gmock_main)
    set_target_properties(${BINARY} PROPERTIES LINKER_LANGUAGE CXX)

    # Add to ctest.
    # add_test(NAME ${BINARY} COMMAND "$<TARGET_FILE:${BINARY}>")
    gtest_discover_tests(${BINARY})

    set(test_targets "${test_targets}" "${testid}")
    if ("${${OUT_VARIABLE}}" STREQUAL "")
        set(${OUT_VARIABLE} "${BINARY}" PARENT_SCOPE)
    else ()
        set(${OUT_VARIABLE} "${${OUT_VARIABLE}}" "${BINARY}" PARENT_SCOPE)
    endif ()

endfunction()

add_subdirectory(CVODEIntegratorTests)
add_subdirectory(SundialsSteadyStateTests)

if (BUILD_PYTHON AND BUILD_TESTS)
    message(STATUS "Building TestModelFactory")
    # temporary: See if this works before cleaning it up
    find_package(Python COMPONENTS Interpreter Development NumPy REQUIRED)
    find_package(SWIG 3.0.0 REQUIRED
            COMPONENTS python
            )
    include(UseSWIG)

    message(STATUS "SWIG_USE_FILE ${SWIG_USE_FILE}")

    set_source_files_properties(TestModelFactory.i
            PROPERTIES
            CPLUSPLUS ON
            SWIG_FLAGS
                # turns on automatic annotation of python arg and return types
                # BUT does so in c++ types. Therefore I'm not sure how much value this adds.
                "-py3"
            )

    swig_add_library(TestModelFactory
            TYPE MODULE
            LANGUAGE python
            SOURCES TestModelFactory.i TestModelFactory.h
            "../../wrappers/Python/roadrunner/PyUtils"
            )

    #    set(CMAKE_SWIG_FLAGS "-py3 -castmode -keyword")
    set_target_properties(TestModelFactory PROPERTIES
            SWIG_USE_TARGET_INCLUDE_DIRECTORIES TRUE
            )

    message(STATUS "Python_INCLUDE_DIRS ${Python_INCLUDE_DIRS}")
    message(STATUS "Python_LIBRARIES ${Python_LIBRARIES}")

    target_include_directories(TestModelFactory PUBLIC
            ${Python_INCLUDE_DIRS}
            ${CMAKE_CURRENT_SOURCE_DIR}
            $<TARGET_PROPERTY:roadrunner-static,INCLUDE_DIRECTORIES>
            $<TARGET_PROPERTY:roadrunner_python,INCLUDE_DIRECTORIES>
            )
    target_link_libraries(TestModelFactory PUBLIC
            roadrunner-static
            ${Python_LIBRARIES}
            )

    install(TARGETS TestModelFactory DESTINATION site-packages/roadrunner/testing)
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/TestModelFactory.py" DESTINATION site-packages/roadrunner/testing)

endif ()




























