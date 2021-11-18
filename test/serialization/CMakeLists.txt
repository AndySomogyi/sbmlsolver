add_subdirectory(PickledSwig)

if (BUILD_TESTS)
    add_test_executable(
            test_state_saving test_targets
            ${SharedTestFiles}
            ../serialization/state_saving.cpp
    )
endif ()


# make test_targets list global to all tests
set(test_targets ${test_targets} PARENT_SCOPE)




