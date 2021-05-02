#add_test_executable(variant_tests test_targets variant_tests.cpp)
add_test_executable(SettingTests test_targets SettingTests.cpp )

## separate executable to test Setting in complete isolation from
## the rest of roadrunner
#add_executable(SettingUnitTests SettingTests.cpp
#        "${RR_ROOT}/source/Setting" # code under test
#        # for "toDoubleVector" in parsing json, which is unfortunately in the wrong place
#        #   (if we're parsing json, we have have a JSONParser, or add the proper dependency)
#        "${RR_ROOT}/source/rrStringUtils"
#        )
#target_include_directories(SettingUnitTests
#        PRIVATE $<TARGET_PROPERTY:SettingTests,INCLUDE_DIRECTORIES>)
#target_link_libraries(SettingUnitTests PRIVATE gtest gtest_main)