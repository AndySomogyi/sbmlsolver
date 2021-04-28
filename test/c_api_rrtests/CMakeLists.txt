add_test_executable(c_api_rrtests_nom_tests test_targets
        ${SharedTestFiles} NomTests.cpp
        )

add_test_executable(c_api_rrtests_test_model test_targets
        ${SharedTestFiles} RRTestFileTests
        )

add_test_executable(c_api_rrtests_other_tests test_targets
        ${SharedTestFiles} OtherRRTestFileTests.cpp
        )

# make test_targets list global to all tests
set(test_targets ${test_targets} PARENT_SCOPE)









