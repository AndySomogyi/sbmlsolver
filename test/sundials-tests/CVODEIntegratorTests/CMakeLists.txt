

set(SHARED_TEST_FILES ../TestModelFactory )

add_test_executable(
        test_cvode_integration test_targets
        CVODEIntegratorIntegrationTest.cpp
        ${SHARED_TEST_FILES}
)

add_test_executable(
        test_cvode_unit test_targets
        CVODEIntegratorUnitTests.cpp
        ${SHARED_TEST_FILES}
)

# make test_targets list global to all tests
set(test_targets ${test_targets} PARENT_SCOPE)


























