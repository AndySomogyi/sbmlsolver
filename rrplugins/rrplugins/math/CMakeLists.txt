# M Totte Karlsson
# J Kyle Medley

set(RRPLUGINS_MATH_SOURCES
  telRandom
  #telMatrix
  telMathUtils
  third_party/mtrand
)

# ** Shared lib ******

if (TLP_BUILD_SHARED_LIB)
  add_library(telplugins_math SHARED ${RRPLUGINS_MATH_SOURCES})

  set_property(TARGET telplugins_math
    PROPERTY COMPILE_DEFINITIONS
    EXPORT_MATH_API
    LIBSBML_USE_CPP_NAMESPACE
    LIBSBML_STATIC
    LIBLAX_STATIC
    STATIC_LIBSTRUCT
    STATIC_NLEQ
    POCO_STATIC
    POCO_NO_AUTOMATIC_LIBS
  )

  target_link_libraries (telplugins_math
    telplugins_common ${RRPLUGINS_COMMON_SHARED_LIBS})

  if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set_target_properties( telplugins_math PROPERTIES INSTALL_RPATH "$ORIGIN/./" )
  endif ()

  # Install shared library
  install(TARGETS telplugins_math
    RUNTIME DESTINATION bin COMPONENT pluginsLib
    LIBRARY DESTINATION lib COMPONENT pluginsLib
    ARCHIVE DESTINATION lib COMPONENT pluginsLib
  )

  if(RR_PLUGINS_INSTALL_TO_RR_SITE_PACKAGES)
    install(TARGETS telplugins_math DESTINATION site-packages/roadrunner)
  endif()
endif()

# ** Static lib ******

if (TLP_BUILD_STATIC_LIB)
  add_library(telplugins_math-static STATIC ${RRPLUGINS_MATH_SOURCES})

  set_property(TARGET telplugins_math-static
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

  target_link_libraries (telplugins_math-static
    telplugins_common-static ${RRPLUGINS_COMMON_STATIC_LIBS})

  # Install static library
  install(TARGETS telplugins_math-static
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