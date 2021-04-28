add_test_executable(
        state_saving test_targets
        ${SharedTestFiles}
        state_saving.cpp
)
# make test_targets list global to all tests
set(test_targets ${test_targets} PARENT_SCOPE)

