add_test_executable(c_api_rrtests_nom_tests test_targets
        ${SharedTestFiles} NomTests.cpp
        )

add_test_executable(c_api_rrtests_test_model test_targets
        ${SharedTestFiles} RRTestFileTests
        )

add_test_executable(c_api_rrtests_other_tests test_targets
        ${SharedTestFiles} OtherRRTestFileTests.cpp
        )









