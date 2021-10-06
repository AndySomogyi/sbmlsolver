add_subdirectory(PickledSwig)

add_test_executable(
        test_state_saving test_targets
        ${SharedTestFiles}
        ../serialization/state_saving.cpp
)

file(COPY pickle_tests.py DESTINATION "${RR_PYTHON_TESTING_BUILD_PREFIX}")
configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/pickle_tests.py"
        "${RR_PYTHON_TESTING_BUILD_PREFIX}/pickle_tests.py"
        COPYONLY
)

# make test_targets list global to all tests
set(test_targets ${test_targets} PARENT_SCOPE)




