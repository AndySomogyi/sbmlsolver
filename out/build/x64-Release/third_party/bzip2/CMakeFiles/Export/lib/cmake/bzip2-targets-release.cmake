#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "BZip2::BZip2::BZip2" for configuration "Release"
set_property(TARGET BZip2::BZip2::BZip2 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(BZip2::BZip2::BZip2 PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/bz2.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/bz2.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS BZip2::BZip2::BZip2 )
list(APPEND _IMPORT_CHECK_FILES_FOR_BZip2::BZip2::BZip2 "${_IMPORT_PREFIX}/lib/bz2.lib" "${_IMPORT_PREFIX}/bin/bz2.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
