# M Totte Karlsson
# J Kyle Medley
# Debashish Roy

set(target telplugins_core)

set(
        sources
        ${TLP_ROOT}/rrplugins/pluginBaseClass/telCPlugin
        ${TLP_ROOT}/rrplugins/pluginBaseClass/telCPPPlugin
        ${TLP_ROOT}/rrplugins/pluginBaseClass/telPlugin
        tel_api   #struct of roadrunner function pointer
        telPluginManager
        telVersionInfo
)

# ** Shared lib ******

if (TLP_BUILD_SHARED_LIB)
    add_library(${target} SHARED ${sources})

    target_compile_definitions(${target} PRIVATE EXPORT_CORE_API)

    target_link_libraries(${target}
            telplugins_common telpluginsBaseClass
            ${RRPLUGINS_COMMON_SHARED_LIBS}
            roadrunner_c_api roadrunner
            )

    target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${TLP_ROOT})

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
    add_library(${target}-static STATIC ${sources})

    target_link_libraries(${target}-static
            telplugins_common-static
            telpluginsBaseClass-static
            ${RRPLUGINS_COMMON_STATIC_LIBS} roadrunner_c_api roadrunner-static)
    target_include_directories(${target}-static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${TLP_ROOT})

    # Install static library
    install(TARGETS ${target}-static
            LIBRARY DESTINATION lib COMPONENT pluginsLib
            ARCHIVE DESTINATION lib COMPONENT pluginsLib
            )
endif ()


