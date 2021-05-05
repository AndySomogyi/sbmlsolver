add_subdirectory(CVODEIntegratorTests)
add_subdirectory(SundialsSteadyStateTests)

if (BUILD_PYTHON AND BUILD_TESTS)
    message(STATUS "Building TestModelFactory")
    add_swig_executable(TestModelFactory TestModelFactory.i
            TestModelFactory
            "../../wrappers/Python/roadrunner/PyUtils"
            )

    configure_file(testTestModelFactory.py "${RR_PYTHON_TESTING_DIR}" COPYONLY)

    # install python test files to install tree
    install(TARGETS TestModelFactory DESTINATION site-packages/roadrunner/testing)
    install(
            FILES
                "${CMAKE_CURRENT_BINARY_DIR}/TestModelFactory.py"
                "${CMAKE_CURRENT_SOURCE_DIR}/testTestModelFactory.py"
            DESTINATION
            site-packages/roadrunner/testing
    )

endif ()


# make test_targets list global to all tests
set(test_targets ${test_targets} PARENT_SCOPE)


























