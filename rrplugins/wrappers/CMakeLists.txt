# M Totte Karlsson
# J Kyle Medley

if(RR_PLUGINS_BUILD_PLUGINS_C_API)
  add_subdirectory("C")
endif()

if(RR_PLUGINS_BUILD_PLUGINS_PYTHON_API)
  add_subdirectory("python")
endif()


