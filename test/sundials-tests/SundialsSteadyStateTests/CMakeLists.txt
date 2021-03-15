

set(SHARED_TEST_FILES )

add_test_executable(KinsolSteadyStateSolverTests test_targets KinsolSteadyStateSolverTests.cpp ${SHARED_TEST_FILES})
add_test_executable(FixedPointIterationTests test_targets FixedPointIterationTests.cpp ${SHARED_TEST_FILES})
add_test_executable(NewtonIterationIntegrationTests test_targets NewtonIterationIntegrationTests.cpp ${SHARED_TEST_FILES})
add_test_executable(NewtonIterationUnitTests test_targets NewtonIterationUnitTests.cpp ${SHARED_TEST_FILES})
add_test_executable(SVDTests test_targets SVDTests.cpp ${SHARED_TEST_FILES})
add_test_executable(PresimulationDecoratorTests test_targets PresimulationDecoratorTests.cpp ${SHARED_TEST_FILES})
add_test_executable(ApproxSteadyStateDecoratorTests test_targets ApproxSteadyStateDecoratorTests.cpp ${SHARED_TEST_FILES})

























