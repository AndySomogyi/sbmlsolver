# M Totte Karlsson
# J Kyle Medley

set(target telplugins_math)


# question: telMatrix is commented out. Does that mean these source files should be deleted?
set(RRPLUGINS_MATH_SOURCES
        telRandom
        #telMatrix
        telMathUtils
        third_party/mtrand
        )
# ** Shared lib ******

if (TLP_BUILD_SHARED_LIB)
    add_library(${target} SHARED ${RRPLUGINS_MATH_SOURCES})
    target_compile_definitions(${target} PRIVATE EXPORT_CORE_API)

    target_link_libraries(${target}
            telplugins_common ${RRPLUGINS_COMMON_SHARED_LIBS})

    if (UNIX)
        set_target_properties(${target} PROPERTIES INSTALL_RPATH "$ORIGIN/./")
    endif ()

    # Install shared library
    install(TARGETS ${target}
            RUNTIME DESTINATION bin COMPONENT pluginsLib
            LIBRARY DESTINATION lib COMPONENT pluginsLib
            ARCHIVE DESTINATION lib COMPONENT pluginsLib
            )

    if (RR_PLUGINS_INSTALL_TO_RR_SITE_PACKAGES)
        install(TARGETS ${target} DESTINATION site-packages/roadrunner)
    endif ()
endif ()

# ** Static lib ******

if (TLP_BUILD_STATIC_LIB)
    add_library(${target}-static STATIC ${RRPLUGINS_MATH_SOURCES})
    target_compile_definitions(${target}-static PRIVATE STATIC_PLUGIN_API)

    target_link_libraries(${target}-static
            telplugins_common-static ${RRPLUGINS_COMMON_STATIC_LIBS})

    # Install static library
    install(TARGETS ${target}-static
            LIBRARY DESTINATION lib COMPONENT pluginsLib
            ARCHIVE DESTINATION lib COMPONENT pluginsLib
            )
endif ()

if (${MSVC})
    # Disable some VS Warnings
    # 4251 About exporting std classes
    # 4018 Comparing unsigned/signed ints
    # 4996 Deprecated functions
    add_definitions("/W3 /D_CRT_SECURE_NO_WARNINGS /wd4251 /wd4018 /wd4996 /nologo")
endif ()

#[[
if(BUILD_TESTS)
    add_subdirectory(testing)
endif()
]]