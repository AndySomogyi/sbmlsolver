#add_test_executable(variant_tests test_targets variant_tests.cpp)

add_test_executable(SettingTests test_targets SettingTests.cpp)
add_test_executable(RoadRunnerAPITests test_targets RoadRunnerAPITests.cpp ${SharedTestFiles})
add_test_executable(GillespieTests test_targets GillespieTests.cpp ${SharedTestFiles})

if (BUILD_PYTHON)
    add_swig_executable(SettingTestsSwigAPI
            SettingTestsSwigAPI.i
            SettingTestsSwigAPI
            "../../wrappers/Python/roadrunner/PyUtils"
            )
endif ()













