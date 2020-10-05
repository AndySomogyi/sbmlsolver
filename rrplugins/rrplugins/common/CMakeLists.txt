# M Totte Karlsson
# J Kyle Medley

set(RRPLUGINS_COMMON_SOURCES
    telArrayList.cpp
    telArrayedParameter.cpp
    telConstants.cpp
    telException.cpp
    telFileName.cpp
    telIniFile.cpp
    telIniKey.cpp
    telIniSection.cpp
    telLogger.cpp
    telProperties.cpp
    telPropertyBase.cpp
    telStringBuilder.cpp
    telStringList.cpp
    telStringListContainer.cpp
    telStringUtils.cpp
    telTelluriumData.cpp
    telUtils.cpp
    telArrayList.h
    telArrayedParameter.h
    telConstants.h
    telException.h
    telFileName.h
    telIniFile.h
    telIniKey.h
    telIniSection.h
    telLogger.h
    telProperties.h
    telPropertyBase.h
    telStringBuilder.h
    telStringList.h
    telStringListContainer.h
    telStringUtils.h
    telTelluriumData.h
    telUtils.h)

# ** Shared lib ******

if (TLP_BUILD_SHARED_LIB)
  add_library(telplugins_common SHARED ${RRPLUGINS_COMMON_SOURCES})

  set_property(TARGET telplugins_common
    PROPERTY COMPILE_DEFINITIONS
    EXPORT_COMMON_API
    LIBSBML_USE_CPP_NAMESPACE
    LIBLAX_STATIC
    STATIC_LIBSTRUCT
    STATIC_NLEQ
    POCO_STATIC
    POCO_NO_AUTOMATIC_LIBS
  )

  target_link_libraries (telplugins_common ${RRPLUGINS_COMMON_SHARED_LIBS})

  target_link_libraries(telplugins_common ${LIBXML2_LIBRARIES})

  if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set_target_properties( telplugins_common PROPERTIES INSTALL_RPATH "$ORIGIN/./" )
  endif ()

  # Install shared library
  install(TARGETS telplugins_common
          RUNTIME DESTINATION bin COMPONENT pluginsLib
          LIBRARY DESTINATION lib COMPONENT pluginsLib
          ARCHIVE DESTINATION lib COMPONENT pluginsLib)

  if(RR_PLUGINS_INSTALL_TO_RR_SITE_PACKAGES)
    install(TARGETS telplugins_common DESTINATION site-packages/roadrunner)
  endif()
endif()

# ** Static lib ******

if (TLP_BUILD_STATIC_LIB)
  add_library(telplugins_common-static STATIC ${RRPLUGINS_COMMON_SOURCES})

  set_property(TARGET telplugins_common-static
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

  target_link_libraries (telplugins_common-static ${RRPLUGINS_COMMON_STATIC_LIBS})

  # Install static library
  install(TARGETS telplugins_common-static
    LIBRARY DESTINATION lib COMPONENT pluginsLib
    ARCHIVE DESTINATION lib COMPONENT pluginsLib
    )
endif()



if(${MSVC})
  # Disable some VS Warnings
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