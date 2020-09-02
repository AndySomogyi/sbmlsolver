#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "zlib::zlibstatic" for configuration "Release"
set_property(TARGET zlib::zlibstatic APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(zlib::zlibstatic PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/zlibstatic.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS zlib::zlibstatic )
list(APPEND _IMPORT_CHECK_FILES_FOR_zlib::zlibstatic "${_IMPORT_PREFIX}/lib/zlibstatic.lib" )

# Import target "zlib::zlib" for configuration "Release"
set_property(TARGET zlib::zlib APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(zlib::zlib PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/zlib.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/zlib1.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS zlib::zlib )
list(APPEND _IMPORT_CHECK_FILES_FOR_zlib::zlib "${_IMPORT_PREFIX}/lib/zlib.lib" "${_IMPORT_PREFIX}/bin/zlib1.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
