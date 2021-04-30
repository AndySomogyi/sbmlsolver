#add_test_executable(
#        test_semantic_STS test_targets
#        ${SharedTestFiles}
#        sbml_test_suite.cpp
#)
# make test_targets list global to all tests
set(test_targets ${test_targets} PARENT_SCOPE)

