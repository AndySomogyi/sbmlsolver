
add_subdirectory(common)

add_subdirectory(math)

add_subdirectory(pluginBaseClass)

add_subdirectory(core)


install(TARGETS roadrunner_c_api 
    DESTINATION "${RR_PLUGINS_PYLIB_INSTALL_PREFIX}"
    COMPONENT rrplugins)
