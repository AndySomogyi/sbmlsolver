##########################################################
#   Library for containing mock objects only
#

add_library(
        rr-mockups OBJECT
        MockExecutableModel.h
        MockSolver.h
)
set(MOCKUPS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(
        rr-mockups PUBLIC
        ${MOCKUPS_DIRECTORY}
)

target_link_libraries(rr-mockups PUBLIC
        roadrunner-static gmock gmock_main gtest gtest_main
        )
































