add_test_executable(
        SBMLTestSuite test_targets
        ${SharedTestFiles}
        sbml_test_suite.cpp
)
# make test_targets list global to all tests
set(test_targets ${test_targets} PARENT_SCOPE)

