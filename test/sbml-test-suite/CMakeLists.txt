add_test_executable(STSTest test_targets STSTest.cpp STS.h STS.cpp)

set(test_targets ${test_targets} PARENT_SCOPE)
