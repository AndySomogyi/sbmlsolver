

set(SHARED_TEST_FILES ../TestModelFactory SundialsSteadyStateSolverUnitTest)

## FixedPointIteration not finished due to difficulties getting it to work.
#add_test_executable(
#        FixedPointIterationTests test_targets
#        FixedPointIterationUnitTests.cpp SundialsSteadyStateSolverUnitTest ${SHARED_TEST_FILES}
#)

add_test_executable(
        ApproxSteadyStateDecoratorUnitTests test_targets
        ApproxSteadyStateDecoratorTests.cpp ${SHARED_TEST_FILES}
)

add_test_executable(
        AutomaticMoietyConservationTests test_targets
        AutomaticMoietyConservationTests.cpp
        SundialsSteadyStateSolverUnitTest
        ${SHARED_TEST_FILES}
)


add_test_executable(
        BasicNewtonIterationUnitTests test_targets
        BasicNewtonIterationUnitTests.cpp
        SundialsSteadyStateSolverUnitTest
        ${SHARED_TEST_FILES}
)

add_test_executable(
        PresimulationDecoratorUnitTests test_targets
        PresimulationDecoratorTests.cpp ${SHARED_TEST_FILES}
)

add_test_executable(
        SVDUnitTests test_targets SVDTests.cpp ${SHARED_TEST_FILES}
)

add_test_executable(
        SteadyStateIntegrationTests test_targets
        SteadyStateIntegrationTests.h
        BasicNewtonIntegrationTests.cpp
        DecoratorIntegrationTests.cpp
        LinesearchNewtonIntegrationTests.cpp
        NLEQ1IntegrationTests.cpp
        NLEQ2IntegrationTests.cpp
        ${SHARED_TEST_FILES}
)

#
## copy Python tests to binary tree site packages
#configure_file(
#        "${CMAKE_CURRENT_SOURCE_DIR}/SteadyStateSolverTests.py"
#        "${CMAKE_BINARY_DIR}/lib/site-packages/roadrunner/testing/SteadyStateSolverTests.py"
#        COPYONLY
#)
#configure_file(
#        "${CMAKE_CURRENT_SOURCE_DIR}/AutomaticMoietyConservationAnalysisTests.py"
#        "${CMAKE_BINARY_DIR}/lib/site-packages/roadrunner/testing/"
#        COPYONLY
#)
#
## install python tests to install tree
#install(
#        FILES
#            "${CMAKE_CURRENT_SOURCE_DIR}/SteadyStateSolverTests.py"
#            "${CMAKE_CURRENT_SOURCE_DIR}/AutomaticMoietyConservationAnalysisTests.py"
#        DESTINATION
#            site-packages/roadrunner/testing
#        )
#





#add_test_executable(
#        SteadyStateIntegrationTests test_targets
#        SteadyStateIntegrationTests.h
##        BasicNewtonIntegrationTests.cpp
##        DecoratorIntegrationTests.cpp
##        LinesearchNewtonIntegrationTests.cpp
##        NLEQ1IntegrationTests.cpp
##        NLEQ2IntegrationTests.cpp
#        ${SHARED_TEST_FILES}
#)




# make test_targets list global to all tests
set(test_targets ${test_targets} PARENT_SCOPE)














