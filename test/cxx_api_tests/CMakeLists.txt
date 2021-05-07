#add_test_executable(variant_tests test_targets variant_tests.cpp)

set(shared_sources ../sundials-tests/TestModelFactory)
add_test_executable(SettingTests test_targets SettingTests.cpp)
add_test_executable(RoadRunnerAPITests test_targets RoadRunnerAPITests.cpp ${shared_sources})
add_test_executable(GillespieTests test_targets GillespieTests.cpp ${shared_sources})

if (BUILD_PYTHON)
    add_swig_executable(SettingTestsSwigAPI
            SettingTestsSwigAPI.i
            SettingTestsSwigAPI
            "../../wrappers/Python/roadrunner/PyUtils"
            )
endif ()













