add_test_executable(
        test_state_saving test_targets
        ${SharedTestFiles}
        state_saving.cpp
)

add_test_executable(
        test_VoidStarToBinary test_targets ${SharedTestFiles}  VoidStarToBinary.cpp
)

# make test_targets list global to all tests
set(test_targets ${test_targets} PARENT_SCOPE)

