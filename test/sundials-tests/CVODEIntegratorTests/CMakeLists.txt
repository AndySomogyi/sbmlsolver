

set(SHARED_TEST_FILES ../TestModelFactory )

add_test_executable(
        CVODEIntegratorIntegrationTest test_targets
        CVODEIntegratorIntegrationTest.cpp
        ${SHARED_TEST_FILES}
)

# make test_targets list global to all tests
set(test_targets ${test_targets} PARENT_SCOPE)


























