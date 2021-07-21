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
add_test_executable(test_cxx_api_JabobianTests test_targets JabobianTests.cpp ${SharedTestFiles})
add_test_executable(test_cxx_api_StructuralAnalysisTests test_targets StructuralAnalysisTests.cpp ${SharedTestFiles})

if (BUILD_PYTHON AND BUILD_TESTS)
    add_swig_executable(SettingTestsSwigAPI
            SettingTestsSwigAPI.i
            SettingTestsSwigAPI
            "../../wrappers/Python/roadrunner/PyUtils"
            )

    configure_file(
            "${CMAKE_CURRENT_SOURCE_DIR}/SettingTests.py"
            "${CMAKE_BINARY_DIR}/lib/site-packages/roadrunner/testing/SettingTests.py"
            COPYONLY
    )

    configure_file(
            "${CMAKE_CURRENT_SOURCE_DIR}/StructuralAnalysisTests.py"
            "${CMAKE_BINARY_DIR}/lib/site-packages/roadrunner/testing/StructuralAnalysisTests.py"
            COPYONLY
    )

    install(FILES
            "${CMAKE_BINARY_DIR}/lib/site-packages/roadrunner/testing/SettingTests.py"
            "${CMAKE_BINARY_DIR}/lib/site-packages/roadrunner/testing/StructuralAnalysisTests.py"
            DESTINATION
            site-packages/roadrunner/testing
            )

endif ()















