#add_test_executable(variant_tests test_targets variant_tests.cpp)


add_test_executable(SettingTests test_targets SettingTests.cpp)

if (BUILD_PYTHON)
    set(dot_i_files "rr_setting.i" "SettingTestsSwigAPI.i")
    add_swig_executable(SettingTestsSwigAPI
            SettingTestsSwigAPI.i
#            ${dot_i_files}
            SettingTestsSwigAPI
            "../../wrappers/Python/roadrunner/PyUtils"
            )
endif ()













