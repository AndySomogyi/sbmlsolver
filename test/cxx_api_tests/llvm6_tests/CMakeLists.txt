###########################################################
#   Tests for LLVM part of roadrunner.
#
#  Note: These were written some years after the initial code was written
#

# place to mess around, not real tests. Delete.
add_test_executable(not-tests test_targets not-tests.cpp ${SharedTestFiles})
add_test_executable(LLVMModelGeneratorTests test_targets LLVMModelGeneratorTests.cpp ${SharedTestFiles})


















