add_subdirectory(googletest)



add_executable(CanILinkRoadrunnerStatic main.cpp)

target_link_libraries(CanILinkRoadrunnerStatic PRIVATE roadrunner-static)
add_dependencies(CanILinkRoadrunnerStatic roadrunner-static)

add_executable(quick_test quick_test.cpp)
target_link_libraries(quick_test PRIVATE roadrunner-static)

#turn off optimizations for debug builds so profiler results do not get skewed
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fno-omit-frame-pointer -fsanitize=address")
set (CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address")










