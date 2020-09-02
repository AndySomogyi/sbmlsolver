# libxml2-config.cmake
# --------------------
#
# Libxml2 cmake module.
# THis module sets the following variables:
#
# ::
#
#   LIBXML2_INCLUDE_DIRS      - Directory where libxml2 headers are located.
#   LIBXML2_LIBRARIES         - xml2 libraries to link against.
#   LIBXML2_VERSION_MAJOR     - The major version of libxml2.
#   LIBXML2_VERSION_MINOR     - The minor version of libxml2.
#   LIBXML2_VERSION_PATCH     - The patch version of libxml2.
#   LIBXML2_VERSION_STRING    - version number as a string (ex: "2.3.4")
#   LIBXML2_MODULES           - whether libxml2 has dso support

include("${CMAKE_CURRENT_LIST_DIR}/libxml2-export.cmake")


####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was libxml2-config.cmake.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

set(LIBXML2_VERSION_MAJOR  2)
set(LIBXML2_VERSION_MINOR  9)
set(LIBXML2_VERSION_PATCH  10)
set(LIBXML2_VERSION_STRING "2.9.10")
set(LIBXML2_INSTALL_PREFIX ${PACKAGE_PREFIX_DIR})
set(LIBXML2_INCLUDE_DIRS   ${PACKAGE_PREFIX_DIR}/include ${PACKAGE_PREFIX_DIR}/include/libxml2)
set(LIBXML2_LIBRARY_DIR    ${PACKAGE_PREFIX_DIR}/lib)

macro(select_library_location target basename)
  foreach(property IN ITEMS IMPORTED_LOCATION IMPORTED_IMPLIB)
    get_target_property(${basename}_${property}_DEBUG ${target} ${property}_DEBUG)
    get_target_property(${basename}_${property}_MINSIZEREL ${target} ${property}_MINSIZEREL)
    get_target_property(${basename}_${property}_RELEASE ${target} ${property}_RELEASE)
    get_target_property(${basename}_${property}_RELWITHDEBINFO ${target} ${property}_RELWITHDEBINFO)
    
    if(${basename}_${property}_DEBUG AND ${basename}_${property}_RELEASE)
      set(${basename}_LIBRARIES debug ${${basename}_${property}_DEBUG} optimized ${${basename}_${property}_RELEASE})
    elseif(${basename}_${property}_DEBUG AND ${basename}_${property}_RELWITHDEBINFO)
      set(${basename}_LIBRARIES debug ${${basename}_${property}_DEBUG} optimized ${${basename}_${property}_RELWITHDEBINFO})
    elseif(${basename}_${property}_DEBUG AND ${basename}_${property}_MINSIZEREL)
      set(${basename}_LIBRARIES debug ${${basename}_${property}_DEBUG} optimized ${${basename}_${property}_MINSIZEREL})
    elseif(${basename}_${property}_RELEASE)
      set(${basename}_LIBRARIES ${${basename}_${property}_RELEASE})
    elseif(${basename}_${property}_RELWITHDEBINFO)
      set(${basename}_LIBRARIES ${${basename}_${property}_RELWITHDEBINFO})
    elseif(${basename}_${property}_MINSIZEREL)
      set(${basename}_LIBRARIES ${${basename}_${property}_MINSIZEREL})
    elseif(${basename}_${property}_DEBUG)
      set(${basename}_LIBRARIES ${${basename}_${property}_DEBUG})
    endif()
  endforeach()
endmacro()

select_library_location(LibXml2::LibXml2 LIBXML2)

include(CMakeFindDependencyMacro)

if(ON)
  find_dependency(Iconv)
  list(APPEND LIBXML2_LIBRARIES    ${Iconv_LIBRARIES})
  list(APPEND LIBXML2_INCLUDE_DIRS ${Iconv_INCLUDE_DIRS})
endif()

if(NOT ON)
  if(ON)
    find_dependency(Threads)
    list(APPEND LIBXML2_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})
  endif()

  if(OFF)
    find_dependency(ICU)
    list(APPEND LIBXML2_LIBRARIES    ${ICU_LIBRARIES})
    list(APPEND LIBXML2_INCLUDE_DIRS ${ICU_INCLUDE_DIRS})
  endif()

  if(ON)
    find_dependency(LibLZMA)
    list(APPEND LIBXML2_LIBRARIES    ${LIBLZMA_LIBRARIES})
    list(APPEND LIBXML2_INCLUDE_DIRS ${LIBLZMA_INCLUDE_DIRS})
  endif()

  if(ON)
    find_dependency(ZLIB)
    list(APPEND LIBXML2_LIBRARIES    ${ZLIB_LIBRARIES})
    list(APPEND LIBXML2_INCLUDE_DIRS ${ZLIB_INCLUDE_DIRS})
  endif()

  if(UNIX)
    list(APPEND LIBXML2_LIBRARIES m)
  endif()

  if(WIN32)
    list(APPEND LIBXML2_LIBRARIES ws2_32)
  endif()
endif()

# whether libxml2 has dso support
set(LIBXML2_MODULES ON)

mark_as_advanced( LIBXML2_INCLUDE_DIRS LIBXML2_LIBRARIES )
