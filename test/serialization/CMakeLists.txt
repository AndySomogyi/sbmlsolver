add_subdirectory(PickledSwig)

add_test_executable(
        test_state_saving test_targets
        ${SharedTestFiles}
        ../serialization/state_saving.cpp
)

file(COPY pickle_tests.py DESTINATION "${RR_PYTHON_TESTING_BUILD_PREFIX}")

# make test_targets list global to all tests
set(test_targets ${test_targets} PARENT_SCOPE)




