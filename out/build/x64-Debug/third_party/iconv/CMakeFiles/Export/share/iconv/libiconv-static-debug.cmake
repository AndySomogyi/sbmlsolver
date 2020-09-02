#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "iconv::libcharset" for configuration "Debug"
set_property(TARGET iconv::libcharset APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(iconv::libcharset PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libcharsetd.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS iconv::libcharset )
list(APPEND _IMPORT_CHECK_FILES_FOR_iconv::libcharset "${_IMPORT_PREFIX}/lib/libcharsetd.lib" )

# Import target "iconv::libiconv-static" for configuration "Debug"
set_property(TARGET iconv::libiconv-static APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(iconv::libiconv-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libiconv-staticd.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS iconv::libiconv-static )
list(APPEND _IMPORT_CHECK_FILES_FOR_iconv::libiconv-static "${_IMPORT_PREFIX}/lib/libiconv-staticd.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
