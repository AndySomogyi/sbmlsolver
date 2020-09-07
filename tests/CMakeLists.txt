add_subdirectory(googletest)



add_executable(CanILinkRoadrunnerStatic main.cpp)

target_link_libraries(CanILinkRoadrunnerStatic PRIVATE roadrunner-static)
add_dependencies(CanILinkRoadrunnerStatic roadrunner-static)










