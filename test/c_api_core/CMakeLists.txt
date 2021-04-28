
add_test_executable(c_api_core_tests test_targets
        rrSBMLTestSuiteSimulation_CAPI
        ${SharedTestFiles}
        CAPICoreTest.cpp
        )


add_test_executable(c_api_core_exceptions test_targets
        rrSBMLTestSuiteSimulation_CAPI
        ${SharedTestFiles}
        exceptions.cpp
        CAPICoreTest
        )


add_test_executable(c_api_core_libstruct test_targets
        rrSBMLTestSuiteSimulation_CAPI
        ${SharedTestFiles}
        libstruct.cpp
        CAPICoreTest
        )


add_test_executable(c_api_core_model_editing test_targets
        rrSBMLTestSuiteSimulation_CAPI
        ${SharedTestFiles}
        model_editing.cpp
        )

# make test_targets list global to all tests
set(test_targets ${test_targets} PARENT_SCOPE)









