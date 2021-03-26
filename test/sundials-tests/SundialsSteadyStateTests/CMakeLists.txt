

set(SHARED_TEST_FILES ../TestModelFactory SteadyStateIntegrationTests)

#add_test_executable(
#        FixedPointIterationTests test_targets
#        FixedPointIterationUnitTests.cpp SundialsSteadyStateSolverUnitTest ${SHARED_TEST_FILES}
#)
add_test_executable(
        SteadyStateIntegrationTests test_targets
        SteadyStateIntegrationTests.cpp ${SHARED_TEST_FILES}
)
add_test_executable(
        BasicNewtonIterationUnitTests test_targets
        BasicNewtonIterationUnitTests.cpp SundialsSteadyStateSolverUnitTest ${SHARED_TEST_FILES}
)
add_test_executable(
        SVDUnitTests test_targets SVDTests.cpp ${SHARED_TEST_FILES}
)
add_test_executable(
        PresimulationDecoratorUnitTests test_targets
        PresimulationDecoratorTests.cpp ${SHARED_TEST_FILES}
)
add_test_executable(
        ApproxSteadyStateDecoratorUnitTests test_targets
        ApproxSteadyStateDecoratorTests.cpp ${SHARED_TEST_FILES}
)

























