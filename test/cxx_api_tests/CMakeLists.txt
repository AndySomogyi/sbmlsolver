
add_test_executable(
        rrLoggerTests test_targets
        rrLoggerTests.cpp
)
# make test_targets list global to all tests
set(test_targets ${test_targets} PARENT_SCOPE)
