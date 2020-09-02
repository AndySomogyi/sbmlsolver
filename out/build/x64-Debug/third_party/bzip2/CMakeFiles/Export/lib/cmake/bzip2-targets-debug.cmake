#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "BZip2::BZip2::BZip2" for configuration "Debug"
set_property(TARGET BZip2::BZip2::BZip2 APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(BZip2::BZip2::BZip2 PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/bz2d.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/bz2d.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS BZip2::BZip2::BZip2 )
list(APPEND _IMPORT_CHECK_FILES_FOR_BZip2::BZip2::BZip2 "${_IMPORT_PREFIX}/lib/bz2d.lib" "${_IMPORT_PREFIX}/bin/bz2d.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
