add_test_executable(c_api_core test_targets
        rrSBMLTestSuiteSimulation_CAPI
        ../RoadRunnerTest
        ${SharedTestFiles}
        core.cpp
        model_editing.cpp
        exceptions.cpp
        )
#exceptions.cpp libstruct.cpp rrSBMLTestSuiteSimulation_CAPI)