#add_test_executable(
#        test_model_analysis test_targets
#        ${SharedTestFiles}
#        model_analysis.cpp
#)

# make test_targets list global to all tests
set(test_targets ${test_targets} PARENT_SCOPE)
