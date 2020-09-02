#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "libsbml" for configuration "Debug"
set_property(TARGET libsbml APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(libsbml PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/libsbmld.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/libsbmld.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS libsbml )
list(APPEND _IMPORT_CHECK_FILES_FOR_libsbml "${_IMPORT_PREFIX}/lib/libsbmld.lib" "${_IMPORT_PREFIX}/bin/libsbmld.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
