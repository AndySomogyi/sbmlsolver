# Debashish
# M Totte Karlsson
# J Kyle Medley

set(target telpluginsBaseClass)

set(sources
  telCPlugin
  telCPPPlugin
  telPlugin
  telVersionInfo
)

# ** Shared lib ******

if (TLP_BUILD_SHARED_LIB)
  add_library(${target} SHARED ${sources})
  target_compile_definitions(${target} PRIVATE EXPORT_CORE_API)

  target_link_libraries (${target} PUBLIC telplugins_common ${RRPLUGINS_COMMON_SHARED_LIBS})
  if (UNIX)
    set_target_properties( telpluginsBaseClass PROPERTIES INSTALL_RPATH "$ORIGIN/./" )
  endif ()

  target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${TLP_ROOT})

  # Install shared library
  install(TARGETS telpluginsBaseClass
    RUNTIME DESTINATION bin COMPONENT pluginsLib
    LIBRARY DESTINATION lib COMPONENT pluginsLib
    ARCHIVE DESTINATION lib COMPONENT pluginsLib
  )

  if(RR_PLUGINS_INSTALL_TO_RR_SITE_PACKAGES)
    install(TARGETS ${target} DESTINATION site-packages/roadrunner)
  endif()
endif()

# ** Static lib ******

if (TLP_BUILD_STATIC_LIB)
  add_library(${target}-static STATIC ${sources})
  target_compile_definitions(${target}-static PRIVATE STATIC_PLUGIN_API)

  target_link_libraries (${target}-static PUBLIC
    telplugins_common-static ${RRPLUGINS_COMMON_STATIC_LIBS})

  target_include_directories(${target}-static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${TLP_ROOT})

  # Install static library
  install(TARGETS telpluginsBaseClass-static
    LIBRARY DESTINATION lib COMPONENT pluginsLib
    ARCHIVE DESTINATION lib COMPONENT pluginsLib
    )
endif()


