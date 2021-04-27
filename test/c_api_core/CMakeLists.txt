add_test_executable(c_api_core test_targets
        core.cpp
        model_editing.cpp
        rrSBMLTestSuiteSimulation_CAPI
        ../RoadRunnerTest
        ${SharedTestFiles}
        )
#model_editing.cpp exceptions.cpp libstruct.cpp rrSBMLTestSuiteSimulation_CAPI)