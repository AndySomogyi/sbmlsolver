# Debashish
# M Totte Karlsson
# J Kyle Medley

include_directories(
    ${C_API_INCLUDE_ROOT}
    ${RR_ROOT}/source
    ${RR_ROOT}
    ${THIRD_PARTY_INSTALL_FOLDER}/include
)

set(sources

  telCPlugin
  telCPPPlugin
  telPlugin
  telVersionInfo
)

# ** Shared lib ******

if (TLP_BUILD_SHARED_LIB)
  add_library(telpluginsBaseClass SHARED ${sources})

  set_property(TARGET telpluginsBaseClass
    PROPERTY COMPILE_DEFINITIONS
    EXPORT_CORE_API
    LIBSBML_USE_CPP_NAMESPACE
    LIBSBML_STATIC
    LIBLAX_STATIC
    STATIC_LIBSTRUCT
    STATIC_NLEQ
    POCO_STATIC
    POCO_NO_AUTOMATIC_LIBS
  )

  target_link_libraries (telpluginsBaseClass
    telplugins_common ${RRPLUGINS_COMMON_SHARED_LIBS})
  if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set_target_properties( telpluginsBaseClass PROPERTIES INSTALL_RPATH "$ORIGIN/./" )
  endif ()

  # Install shared library
  install(TARGETS telpluginsBaseClass
    RUNTIME DESTINATION bin COMPONENT pluginsLib
    LIBRARY DESTINATION lib COMPONENT pluginsLib
    ARCHIVE DESTINATION lib COMPONENT pluginsLib
  )

  if(RR_PLUGINS_INSTALL_TO_RR_SITE_PACKAGES)
    install(TARGETS telpluginsBaseClass DESTINATION site-packages/roadrunner)
  endif()
endif()

# ** Static lib ******

if (TLP_BUILD_STATIC_LIB)
  add_library(telpluginsBaseClass-static STATIC ${sources})

  set_property(TARGET telpluginsBaseClass-static
    PROPERTY COMPILE_DEFINITIONS
    STATIC_PLUGIN_API
    LIBSBML_USE_CPP_NAMESPACE
    LIBSBML_STATIC
    LIBLAX_STATIC
    STATIC_LIBSTRUCT
    STATIC_NLEQ
    POCO_STATIC
    POCO_NO_AUTOMATIC_LIBS
    )

  target_link_libraries (telpluginsBaseClass-static
    telplugins_common-static ${RRPLUGINS_COMMON_STATIC_LIBS})

  # Install static library
  install(TARGETS telpluginsBaseClass-static
    LIBRARY DESTINATION lib COMPONENT pluginsLib
    ARCHIVE DESTINATION lib COMPONENT pluginsLib
    )
endif()

if(${MSVC})
    #Disable some VS Warnings
    # 4251 About exporting std classes
    # 4018 Comparing unsigned/signed ints
    # 4996 Deprecated functions
    add_definitions( "/W3 /D_CRT_SECURE_NO_WARNINGS /wd4251 /wd4018 /wd4996 /nologo" )

endif()

#[[
if(BUILD_TESTS)
    add_subdirectory(testing)
endif()
]]