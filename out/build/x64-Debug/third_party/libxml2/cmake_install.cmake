# Install script for directory: D:/rrbuild/roadrunner/third_party/libxml2

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "D:/rrbuild/roadrunner/out/install/x64-Debug")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevelopmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libxml2/libxml" TYPE FILE FILES
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/c14n.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/catalog.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/chvalid.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/debugXML.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/dict.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/DOCBparser.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/encoding.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/entities.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/globals.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/hash.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/HTMLparser.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/HTMLtree.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/list.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/nanoftp.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/nanohttp.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/parser.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/parserInternals.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/pattern.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/relaxng.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/SAX.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/SAX2.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/schemasInternals.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/schematron.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/threads.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/tree.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/uri.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/valid.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/xinclude.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/xlink.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/xmlIO.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/xmlautomata.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/xmlerror.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/xmlexports.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/xmlmemory.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/xmlmodule.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/xmlreader.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/xmlregexp.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/xmlsave.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/xmlschemas.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/xmlschemastypes.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/xmlstring.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/xmlunicode.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/xmlwriter.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/xpath.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/xpathInternals.h"
    "D:/rrbuild/roadrunner/third_party/libxml2/include/libxml/xpointer.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevelopmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/rrbuild/roadrunner/out/build/x64-Debug/lib/libxml2d.lib")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xruntimex" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/rrbuild/roadrunner/out/build/x64-Debug/bin/libxml2d.dll")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdebugx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg]|[Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE FILES "D:/rrbuild/roadrunner/out/build/x64-Debug/bin/libxml2d.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg]|[Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdocumentationx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/man/man3" TYPE FILE FILES "D:/rrbuild/roadrunner/third_party/libxml2/libxml.3")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdocumentationx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/man/man1" TYPE FILE FILES "D:/rrbuild/roadrunner/third_party/libxml2/doc/xmlcatalog.1")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdocumentationx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/man/man1" TYPE FILE FILES "D:/rrbuild/roadrunner/third_party/libxml2/doc/xmllint.1")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdocumentationx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/libxml2" TYPE DIRECTORY FILES "D:/rrbuild/roadrunner/third_party/libxml2/doc/" REGEX "/makefile\\.[^/]*$" EXCLUDE)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevelopmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/libxml2-2.9.10" TYPE FILE FILES "D:/rrbuild/roadrunner/out/build/x64-Debug/third_party/libxml2/libxml2-config.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevelopmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/libxml2-2.9.10" TYPE FILE FILES "D:/rrbuild/roadrunner/out/build/x64-Debug/third_party/libxml2/libxml2-config-version.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevelopmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/libxml2-2.9.10/libxml2-export.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/libxml2-2.9.10/libxml2-export.cmake"
         "D:/rrbuild/roadrunner/out/build/x64-Debug/third_party/libxml2/CMakeFiles/Export/lib/cmake/libxml2-2.9.10/libxml2-export.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/libxml2-2.9.10/libxml2-export-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/libxml2-2.9.10/libxml2-export.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/libxml2-2.9.10" TYPE FILE FILES "D:/rrbuild/roadrunner/out/build/x64-Debug/third_party/libxml2/CMakeFiles/Export/lib/cmake/libxml2-2.9.10/libxml2-export.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/libxml2-2.9.10" TYPE FILE FILES "D:/rrbuild/roadrunner/out/build/x64-Debug/third_party/libxml2/CMakeFiles/Export/lib/cmake/libxml2-2.9.10/libxml2-export-debug.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevelopmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libxml2/libxml" TYPE FILE FILES "D:/rrbuild/roadrunner/out/build/x64-Debug/third_party/libxml2/libxml/xmlversion.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevelopmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libxml2/libxml" TYPE FILE FILES "D:/rrbuild/roadrunner/out/build/x64-Debug/third_party/libxml2/libxml/xmlwin32version.h")
endif()

