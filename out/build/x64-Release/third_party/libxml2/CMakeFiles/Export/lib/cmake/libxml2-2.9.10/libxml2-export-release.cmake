#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "LibXml2::LibXml2" for configuration "Release"
set_property(TARGET LibXml2::LibXml2 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LibXml2::LibXml2 PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libxml2.lib"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "LibLZMA::LibLZMA"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/libxml2.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS LibXml2::LibXml2 )
list(APPEND _IMPORT_CHECK_FILES_FOR_LibXml2::LibXml2 "${_IMPORT_PREFIX}/lib/libxml2.lib" "${_IMPORT_PREFIX}/bin/libxml2.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
