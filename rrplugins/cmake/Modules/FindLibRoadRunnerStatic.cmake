#.rst:
# FindLibRoadRunnerStatic
# -----------
#
# Try to find libRoadRunner, an SBML-compliant simulation library.
# Find static versions of libs.
# Set LIBROADRUNNERSTATIC_PATH to give a hint about where to search.
#
# Once done this will define
#
# ::
#
#   LIBROADRUNNERSTATIC_FOUND - System has libRoadRunner static libs
#   LIBROADRUNNERSTATIC_INCLUDE_DIR - The libRoadRunner include directory
#   LIBROADRUNNERSTATIC_LIBRARIES - The libraries needed to use libRoadRunner
#   LIBROADRUNNERSTATIC_DEFINITIONS - Compiler switches required for using libRoadRunner
#   LIBROADRUNNERSTATIC_VERSION_STRING - the version of libRoadRunner found

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
  ${LIBROADRUNNERSTATIC_PATH}/lib
  )

find_library(POCO_FOUNDATION_LIBRARY_STATIC NAMES PocoFoundation
  HINTS
  ${LIBROADRUNNERSTATIC_PATH}/lib
  )



message(STATUS "Searching for libsbml-static in ${LIBROADRUNNERSTATIC_PATH}/lib")
find_library(LIBROADRUNNERSTATIC_SBML_LIBRARY
  NAMES
  sbml-static
  libsbml-static.lib # Windows
  HINTS
  ${LIBROADRUNNERSTATIC_PATH}/lib
  )
  message(STATUS "lisbml-static = ${LIBROADRUNNERSTATIC_SBML_LIBRARY}")
get_filename_component(LIBROADRUNNERSTATIC_SBML_LIBRARY_BARE ${LIBROADRUNNERSTATIC_SBML_LIBRARY} NAME)
get_filename_component(LIBROADRUNNERSTATIC_SBML_LIBRARY_DIR  ${LIBROADRUNNERSTATIC_SBML_LIBRARY} DIRECTORY)

set ( LIBROADRUNNERSTATIC_LIBRARIES 
    ${RR_LIBSTRUCT_LIBRARY_STATIC};${POCO_FOUNDATION_LIBRARY_STATIC};${LIBROADRUNNERSTATIC_SBML_LIBRARY_BARE} 
    )

message( STATUS ${LIBROADRUNNERSTATIC_LIBRARIES} )

# handle the QUIETLY and REQUIRED arguments and set LIBROADRUNNERSTATIC_FOUND to TRUE if
# all listed variables are TRUE

include(${CMAKE_CURRENT_LIST_DIR}/FindPackageHandleStandardArgs.cmake)

#message(STATUS ${LIBROADRUNNERSTATIC_INCLUDE_DIR})

FIND_PACKAGE_HANDLE_STANDARD_ARGS(LibRoadRunnerStatic
                                  REQUIRED_VARS LIBROADRUNNERSTATIC_LIBRARIES )

mark_as_advanced(LIBROADRUNNERSTATIC_LIBRARIES)

