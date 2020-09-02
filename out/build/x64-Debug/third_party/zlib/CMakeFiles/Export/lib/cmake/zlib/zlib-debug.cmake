#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "zlib::zlibstatic" for configuration "Debug"
set_property(TARGET zlib::zlibstatic APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(zlib::zlibstatic PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/zlibstaticd.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS zlib::zlibstatic )
list(APPEND _IMPORT_CHECK_FILES_FOR_zlib::zlibstatic "${_IMPORT_PREFIX}/lib/zlibstaticd.lib" )

# Import target "zlib::zlib" for configuration "Debug"
set_property(TARGET zlib::zlib APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(zlib::zlib PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/zlibd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/zlibd1.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS zlib::zlib )
list(APPEND _IMPORT_CHECK_FILES_FOR_zlib::zlib "${_IMPORT_PREFIX}/lib/zlibd.lib" "${_IMPORT_PREFIX}/bin/zlibd1.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
