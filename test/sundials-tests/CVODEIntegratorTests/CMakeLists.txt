

set(SHARED_TEST_FILES ../TestModelFactory )

add_test_executable(
        CVODEIntegratorIntegrationTest test_targets
        CVODEIntegratorIntegrationTest.cpp
        ${SHARED_TEST_FILES}
)



























