

add_test_executable(
        test_cvode_integration test_targets
        CvodeIntegrationTest.cpp
        ${SharedTestFiles}
)

add_test_executable(
        test_cvode_unit test_targets
        CvodeUnitTest.cpp
        ${SharedTestFiles}
)

# make test_targets list global to all tests
set(test_targets ${test_targets} PARENT_SCOPE)


























