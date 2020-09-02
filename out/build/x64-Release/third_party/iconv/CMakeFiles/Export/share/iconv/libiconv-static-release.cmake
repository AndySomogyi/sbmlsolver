#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "iconv::libcharset" for configuration "Release"
set_property(TARGET iconv::libcharset APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(iconv::libcharset PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libcharset.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS iconv::libcharset )
list(APPEND _IMPORT_CHECK_FILES_FOR_iconv::libcharset "${_IMPORT_PREFIX}/lib/libcharset.lib" )

# Import target "iconv::libiconv-static" for configuration "Release"
set_property(TARGET iconv::libiconv-static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(iconv::libiconv-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libiconv-static.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS iconv::libiconv-static )
list(APPEND _IMPORT_CHECK_FILES_FOR_iconv::libiconv-static "${_IMPORT_PREFIX}/lib/libiconv-static.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
