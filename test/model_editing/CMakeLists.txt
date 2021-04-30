#add_test_executable(
#        test_model_editing test_targets
#        ${SharedTestFiles}
#        model_editing.cpp
#)

# make test_targets list global to all tests
set(test_targets ${test_targets} PARENT_SCOPE)
