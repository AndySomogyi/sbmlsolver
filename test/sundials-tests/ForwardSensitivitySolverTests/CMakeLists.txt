#add_test_executable(cvsRoberts_FSA_dns cvode-example cvsRoberts_FSA_dns.c)

add_test_executable(test_sundials_FFSUnitTests test_targets FFSUnitTests.cpp)
add_test_executable(test_sundials_FFSIntegrationTests test_targets FFSIntegrationTests.cpp ../CVODEIntegratorTests/CvodeIntegrationTest.h)



if (BUILD_PYTHON AND BUILD_TESTS)
    configure_file(
            "${CMAKE_CURRENT_SOURCE_DIR}/ForwardSensitivitySolverTests.py"
            "${CMAKE_BINARY_DIR}/lib/site-packages/roadrunner/testing/ForwardSensitivitySolverTests.py"
            COPYONLY
    )

    install(FILES
            "${CMAKE_BINARY_DIR}/lib/site-packages/roadrunner/testing/ForwardSensitivitySolverTests.py"
            DESTINATION
            site-packages/roadrunner/testing
            )

endif ()










