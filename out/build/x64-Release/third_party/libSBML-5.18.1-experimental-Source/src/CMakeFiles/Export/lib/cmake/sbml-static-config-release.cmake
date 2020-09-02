#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "sbml-static" for configuration "Release"
set_property(TARGET sbml-static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(sbml-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C;CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/sbml-static.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS sbml-static )
list(APPEND _IMPORT_CHECK_FILES_FOR_sbml-static "${_IMPORT_PREFIX}/lib/sbml-static.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
