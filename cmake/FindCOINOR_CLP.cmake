# FindCOINOR_CLP

# -----------
#
# Try to find the COIN-OR CLP constraint linear programming library
#
# Once done this will define
#
# ::
#
#   COINOR_CLP_FOUND - System has COINOR CLP
#   COINOR_CLP_INCLUDE_DIR - The COINOR CLP include directory
#   COINOR_CLP_LIBRARIES - The libraries needed to use COINOR CLP
#   COINOR_CLP_DEFINITIONS - Compiler switches required for using COINOR CLP
#   COINOR_CLP_VERSION - the version of COINOR CLP found
#
#=============================================================================
# Copyright 2015 Andy Somogyi, andy.somogyi@gmail.com, Indiana University
#
# Based off the FindLibXml2 cmake by Alexander Neundorf <neundorf@kde.org>
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

# use pkg-config to get the directories and then use these values
# in the find_path() and find_library() calls
#find_package(PkgConfig QUIET)
#PKG_CHECK_MODULES(PC_LIBXML QUIET libxml-2.0)
#set(LIBXML2_DEFINITIONS ${PC_LIBXML_CFLAGS_OTHER})

#   ${PC_LIBXML_INCLUDEDIR}
#   ${PC_LIBXML_INCLUDE_DIRS}


find_path(COINOR_CLP_INCLUDE_DIR NAMES coin/CoinUtilsConfig.h
   HINTS
   ${COINOR_PREFIX}
   PATH_SUFFIXES include
   )

message("COINOR_CLP_INCLUDE_DIR: ${COINOR_CLP_INCLUDE_DIR}")

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")

set(COINOR_CLP_LIBRARIES "")

set(_libClp _libClp-NOTFOUND)
find_library(_libClp
  NAMES Clp
  HINTS ${COINOR_PREFIX}/lib
  )

set(_libClpSolver _libClpSolver-NOTFOUND)
find_library(_libClpSolver
  NAMES ClpSolver
  HINTS ${COINOR_PREFIX}/lib
  )



set(_libCoinUtils _libCoinUtils-NOTFOUND)
find_library(_libCoinUtils
  NAMES CoinUtils
  HINTS ${COINOR_PREFIX}/lib
  )

list(APPEND COINOR_CLP_LIBRARIES ${_libCoinUtils})
list(APPEND COINOR_CLP_LIBRARIES ${_libClpSolver})
list(APPEND COINOR_CLP_LIBRARIES ${_libClp})
list(APPEND COINOR_CLP_LIBRARIES ${_libClpSolver})
list(APPEND COINOR_CLP_LIBRARIES ${_libCoinUtils})


message("COINOR_CLP_LIBRARIES: ${COINOR_CLP_LIBRARIES}")

#find_library(LIBXML2_LIBRARIES NAMES xml2 libxml2
#   HINTS
#   ${PC_LIBXML_LIBDIR}
#   ${PC_LIBXML_LIBRARY_DIRS}
#   )

#find_program(LIBXML2_XMLLINT_EXECUTABLE xmllint)
# for backwards compat. with KDE 4.0.x:
#set(XMLLINT_EXECUTABLE "${LIBXML2_XMLLINT_EXECUTABLE}")

#if(PC_LIBXML_VERSION)
#    set(LIBXML2_VERSION_STRING ${PC_LIBXML_VERSION})
#elseif(LIBXML2_INCLUDE_DIR AND EXISTS "${LIBXML2_INCLUDE_DIR}/libxml/xmlversion.h")
#    file(STRINGS "${LIBXML2_INCLUDE_DIR}/libxml/xmlversion.h" libxml2_version_str
#         REGEX "^#define[\t ]+LIBXML_DOTTED_VERSION[\t ]+\".*\"")
#
#    string(REGEX REPLACE "^#define[\t ]+LIBXML_DOTTED_VERSION[\t ]+\"([^\"]*)\".*" "\\1"
#           LIBXML2_VERSION_STRING "${libxml2_version_str}")
#    unset(libxml2_version_str)
#endif()

# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE if
# all listed variables are TRUE
#include(${CMAKE_CURRENT_LIST_DIR}/FindPackageHandleStandardArgs.cmake)
#FIND_PACKAGE_HANDLE_STANDARD_ARGS(LibXml2
#                                  REQUIRED_VARS LIBXML2_LIBRARIES LIBXML2_INCLUDE_DIR
#                                  VERSION_VAR LIBXML2_VERSION_STRING)

#mark_as_advanced(LIBXML2_INCLUDE_DIR LIBXML2_LIBRARIES LIBXML2_XMLLINT_EXECUTABLE)
