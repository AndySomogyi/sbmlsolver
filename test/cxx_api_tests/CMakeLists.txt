#add_test_executable(variant_tests test_targets variant_tests.cpp)

add_test_executable(test_cxx_api_SettingTests test_targets SettingTests.cpp)
add_test_executable(test_cxx_api_RoadRunnerAPITests test_targets RoadRunnerAPITests.cpp ${SharedTestFiles})
add_test_executable(test_cxx_api_GillespieTests test_targets GillespieTests.cpp ${SharedTestFiles})
add_test_executable(test_cxx_api_LoggerTests test_targets LoggerTests.cpp ${SharedTestFiles})
add_test_executable(test_cxx_api_BasicDictionaryTests test_targets BasicDictionaryTests.cpp ${SharedTestFiles})
add_test_executable(test_cxx_api_SelectionRecordTests test_targets SelectionRecordTests.cpp ${SharedTestFiles})
add_test_executable(test_cxx_api_LoadSBMLOptionsTests test_targets LoadSBMLOptionsTests.cpp ${SharedTestFiles})
add_test_executable(test_cxx_api_ConfigTests test_targets ConfigTests.cpp ${SharedTestFiles})
add_test_executable(test_cxx_api_MatrixTests test_targets MatrixTests.cpp ${SharedTestFiles})
add_test_executable(test_cxx_api_Matrix3DTests test_targets Matrix3DTests.cpp ${SharedTestFiles})
add_test_executable(test_cxx_api_JacobianTests test_targets JacobianTests.cpp ${SharedTestFiles})
add_test_executable(test_cxx_api_StructuralAnalysisTests test_targets StructuralAnalysisTests.cpp ${SharedTestFiles})
add_test_executable(test_cxx_api_MCATests test_targets MCATests.cpp ${SharedTestFiles})












