

set(SHARED_TEST_FILES ../TestModelFactory SteadyStateIntegrationTests)

add_test_executable(
        FixedPointIterationTests test_targets
        FixedPointIterationUnitTests.cpp SundialsSteadyStateSolverUnitTest ${SHARED_TEST_FILES}
)
add_test_executable(
        NewtonIterationIntegrationTests test_targets
        NewtonIterationIntegrationTests.cpp ${SHARED_TEST_FILES}
)
add_test_executable(
        NewtonIterationUnitTests test_targets
        BasicNewtonIterationUnitTests.cpp SundialsSteadyStateSolverUnitTest ${SHARED_TEST_FILES}
)
add_test_executable(
        SVDTests test_targets SVDTests.cpp ${SHARED_TEST_FILES}
)
add_test_executable(
        PresimulationDecoratorTests test_targets
        PresimulationDecoratorTests.cpp ${SHARED_TEST_FILES}
)
add_test_executable(
        ApproxSteadyStateDecoratorTests test_targets
        ApproxSteadyStateDecoratorTests.cpp ${SHARED_TEST_FILES}
)

























