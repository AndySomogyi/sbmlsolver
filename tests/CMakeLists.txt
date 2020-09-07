set(target CanILinkRoadrunnerStatic)
add_executable(${target}
        main.cpp
        SBMLFactory.h SBMLFactory.cpp

        )

target_include_directories(${target} PRIVATE
        ${GOOGLETEST_INCLUDE_DIR}
        )

target_link_libraries(${target} PRIVATE roadrunner-static)
add_dependencies(${target} roadrunner-static)
