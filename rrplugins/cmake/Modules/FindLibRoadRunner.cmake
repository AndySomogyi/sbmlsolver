#.rst:
# FindLibRoadRunner
# -----------
#
# Try to find libRoadRunner, an SBML-compliant simulation library.
# Set LIBROADRUNNER_PATH to give a hint about where to search.
#
# Once done this will define
#
# ::
#
#   LIBROADRUNNER_FOUND - System has libRoadRunner
#   LIBROADRUNNER_INCLUDE_DIR - The libRoadRunner include directory
#   LIBROADRUNNER_LIBRARIES - The libraries needed to use libRoadRunner
#   LIBROADRUNNER_DEFINITIONS - Compiler switches required for using libRoadRunner
#   LIBROADRUNNER_VERSION_STRING - the version of libRoadRunner found

#=============================================================================
# Copyright 2015 J. Kyle Medley, Debashsish Roy
#
# Licensed under the Apache License, Version 2.0
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#
# Based on FindLibXml2.cmake
#
#=============================================================================

find_library(RR_LIBSTRUCT_LIBRARY_STATIC NAMES rr-libstruct-static
  HINTS
  ${LIBROADRUNNER_PATH}/lib
  )

find_library(POCO_FOUNDATION_LIBRARY_STATIC NAMES PocoFoundation
  HINTS
  ${LIBROADRUNNER_PATH}/lib
  )

find_library(SBML_LIBRARY
  NAMES
  sbml-static
  libsbml-static.lib # Windows
  HINTS
  ${LIBROADRUNNER_PATH}/lib
  )
  
get_filename_component(SBML_LIBRARY_BARE ${SBML_LIBRARY} NAME)
get_filename_component(SBML_LIBRARY_DIR  ${SBML_LIBRARY} DIRECTORY)

set ( LIBROADRUNNER_LIBRARIES 
      ${POCO_FOUNDATION_LIBRARY_STATIC};${SBML_LIBRARY_BARE};${RR_LIBSTRUCT_LIBRARY_STATIC}
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  find_library(ICONV_LIBRARY
    NAMES
    libiconv.lib
    HINTS
    ${LIBROADRUNNER_PATH}/lib
    )
  find_library(ZLIB_LIBRARY
    NAMES
    zdll.lib
    HINTS
    ${LIBROADRUNNER_PATH}/lib
    )
  list ( APPEND LIBROADRUNNER_LIBRARIES ${ICONV_LIBRARY};${ZLIB_LIBRARY};ws2_32.lib;Iphlpapi.lib )
endif ()
message( STATUS "LIBROADRUNNER_LIBRARIES: ${LIBROADRUNNER_LIBRARIES}" )

# handle the QUIETLY and REQUIRED arguments and set LIBROADRUNNER_FOUND to TRUE if
# all listed variables are TRUE

include(${CMAKE_CURRENT_LIST_DIR}/FindPackageHandleStandardArgs.cmake)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(LibRoadRunner
                                  REQUIRED_VARS LIBROADRUNNER_LIBRARIES)

mark_as_advanced(LIBROADRUNNER_LIBRARIES)
