set(target CanILinkRoadrunnerStatic)
add_executable(${target} main.cpp)

target_include_directories(${target} PRIVATE
        ${INCLUDE_DIRECTORIES}
        #        "D:/rrbuild/roadrunner/source"
        )

target_link_libraries(${target} PRIVATE
        $<TARGET_FILE:roadrunner-static>
        ${LINK_LIBRARIES}
        gtest gtest_main
#        Ws2_32.lib
#        shlwapi.lib
#        shell32.lib
        )

add_dependencies(${target} roadrunner-static)

if (WIN32)
    # Indicate that we want to link to static roadrunner
    target_compile_options(${target} PRIVATE -DSTATIC_RR -DLIBXML_STATIC
        -DLIBSBML_STATIC
        -DLIBLAX_STATIC
        -DSTATIC_LIBSTRUCT
        -DSTATIC_PUGI
        -DSTATIC_NLEQ1
        -DSTATIC_NLEQ2
        -DPOCO_STATIC)
    set_property(TARGET ${target} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")


endif ()