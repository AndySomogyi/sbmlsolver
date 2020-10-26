
add_subdirectory(common)

add_subdirectory(math)

add_subdirectory(pluginBaseClass)

add_subdirectory(core)


install(TARGETS roadrunner_c_api DESTINATION "${PYTHON_PACKAGE_INSTALL_PREFIX}/rrplugins")
