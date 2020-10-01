# M Totte Karlsson
# J Kyle Medley
# Debashish Roy

cmake_minimum_required(VERSION 3.2)
project( rrplugins )

# Add local modules to CMake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/rrplugins/cmake/Modules/")

# Variables used in project

set ( TLP_DEPENDENCIES_PATH ${CMAKE_INSTALL_PREFIX} CACHE PATH "Directory containing ThirdParty libraries.")

set ( TLP_GENERATED_HEADER_PATH ${CMAKE_CURRENT_BINARY_DIR}/rrplugins )
set ( TLP_SOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR} )
set ( TLP_ROOT ${CMAKE_CURRENT_SOURCE_DIR} )

# Global settings
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY     ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY     ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY     ${PROJECT_BINARY_DIR}/bin)

# Check dependencies
if( TLP_DEPENDENCIES_PATH STREQUAL "" )
  message( FATAL_ERROR "Please set TLP_DEPENDENCIES_PATH to point to the directory containing third-party dependencies" )
elseif( NOT IS_DIRECTORY "${TLP_DEPENDENCIES_PATH}/include/Poco" )
  message( FATAL_ERROR "TLP_DEPENDENCIES_PATH is missing include/Poco" )
elseif( NOT EXISTS   "${TLP_DEPENDENCIES_PATH}/include/Poco/Poco.h" OR
        IS_DIRECTORY "${TLP_DEPENDENCIES_PATH}/include/Poco/Poco.h" )
  message( FATAL_ERROR "Could not find Poco.h in TLP_DEPENDENCIES_PATH/include/Poco" )
elseif( NOT EXISTS   "${TLP_DEPENDENCIES_PATH}/include/Poco/Foundation.h" OR
        IS_DIRECTORY "${TLP_DEPENDENCIES_PATH}/include/Poco/Foundation.h" )
  message( FATAL_ERROR "Could not find Foundation.h in TLP_DEPENDENCIES_PATH/include/Poco" )
endif()

# Feature selection
set ( TLP_BUILD_SHARED_LIB ON  CACHE BOOL   "Build shared library" )
set ( TLP_BUILD_STATIC_LIB ON  CACHE BOOL   "Build static library" )
set ( TLP_BUILD_PLUGINS_C_API ON CACHE BOOL      "Build Plugins C API")
set ( TLP_BUILD_PLUGINS_PYTHON_API ON CACHE BOOL "Build Plugins Python API")
set ( RR_PLUGINS_INSTALL_TO_RR_SITE_PACKAGES TRUE CACHE BOOL "Install libs to roadrunner site-packages directory")
set ( RRP_PLUGINS_DIR site-packages/roadrunner CACHE PATH "Destination for plugins")

# Plugin selection: Released

set (TLP_BUILD_TEST_MODEL_PLUGIN ON CACHE BOOL "Build the Test Model Plugin")
set (TLP_BUILD_ADD_NOISE_PLUGIN ON CACHE BOOL "Build the Add Noise Plugin")
set (TLP_BUILD_CHISQUARE_PLUGIN ON CACHE BOOL "Build the ChiSquare Plugin")
#set (TLP_BUILD_LEVENBERG_MARQUARDT_PLUGIN ON CACHE BOOL "Build the Levenberg-Marquardt Plugin")
#set (TLP_BUILD_NELDER_MEAD_PLUGIN OFF CACHE BOOL "Build the Nelder-Mead Plugin")
set (TLP_BUILD_AUTO2000_PLUGIN ON CACHE BOOL "Build the Auto 2000 Plugin")
set (TLP_BUILD_HELLO_PLUGIN ON CACHE BOOL "Build the Demo Plugin")

# Dependencies

# Poco
set ( POCO_DIRECTORY "${TLP_DEPENDENCIES_PATH}" CACHE PATH "Path to directory containing Poco" )
set ( ENV{Poco_DIR} ${POCO_DIRECTORY} )
find_package ( Poco REQUIRED )

# libxml2
set( LIBXML2_PATH "${TLP_DEPENDENCIES_PATH}" )
find_package ( LibXml2 REQUIRED )

# Non llvm dependencies
# - Shared

set( LIBROADRUNNER_PATH "${TLP_DEPENDENCIES_PATH}" )
find_package ( LibRoadRunner REQUIRED )
# - Static
set( LIBROADRUNNERSTATIC_PATH "${TLP_DEPENDENCIES_PATH}" )
find_package ( LibRoadRunnerStatic REQUIRED )


# Threading
if(UNIX)
  find_package(Threads REQUIRED)
endif()

message(STATUS "
-------------------------------------------------------------------------------
rrplugins
-------------------------------------------------------------------------------

    Features:
      Enable shared libs?       ${TLP_BUILD_SHARED_LIB}
      Enable static libs?       ${TLP_BUILD_STATIC_LIB}

    Language bindings:
      Enable C bindings?        ${TLP_BUILD_PLUGINS_C_API}
      Enable python bindings?   ${TLP_BUILD_PLUGINS_PYTHON_API}

    Released plugins:
      Test Model                ${TLP_BUILD_TEST_MODEL_PLUGIN}
      Add Noise                 ${TLP_BUILD_ADD_NOISE_PLUGIN}
      Chi-squared               ${TLP_BUILD_CHISQUARE_PLUGIN}
      Bifurcation Auto2k        ${TLP_BUILD_AUTO2000_PLUGIN}
      Demo Plugin               ${TLP_BUILD_HELLO_PLUGIN}

    Dependencies (packages marked with *** are required for rrplugins):
    "
)

message( " *** libsbml libdir               ${SBML_LIBRARY_DIR}")
message( " *** libsbml                      ${SBML_LIBRARY_BARE}")
link_directories(${SBML_LIBRARY_DIR})


if(LIBROADRUNNER_FOUND)
    message( "  *** third-party libs             ${LIBROADRUNNER_LIBRARIES}")
else()
    message( "  *** roadrunner                  *N/A")
endif()

if(LIBROADRUNNERSTATIC_FOUND)
    message( "  *** third-party static libs      ${LIBROADRUNNERSTATIC_LIBRARIES}")
else()
    message( "  *** roadrunner static           *N/A")
endif()

if(Poco_FOUND)
    message( "  *** Poco libs                   ${Poco_LIBRARIES}")
    message( "  *** Poco includes               ${Poco_INCLUDE_DIRS}")
else()
    message( "  *** Poco                        *N/A")
endif()

if(LIBXML2_FOUND)
    message( "  *** libxml2 libs                ${LIBXML2_LIBRARIES}")
    message( "  *** libxml2 includes            ${LIBXML2_INCLUDE_DIR}")
else()
    message( "  *** libxml2                     *N/A")
endif()

# Common includes
include_directories(
  ${TLP_ROOT}
  ${TLP_ROOT}/manager
  ${Poco_INCLUDE_DIRS}
  ${LIBROADRUNNER_INCLUDE_DIR}
)

if(UNIX)
  set(RRPLUGINS_COMMON_SHARED_LIBS ${LIBROADRUNNER_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} dl)
else()
  if(${MSVC})
    set(RRPLUGINS_COMMON_SHARED_LIBS ${LIBROADRUNNER_LIBRARIES})
  else()
    set(RRPLUGINS_COMMON_SHARED_LIBS ${LIBROADRUNNER_LIBRARIES} iphlpapi)
  endif()
endif()

# Flags
if( MSVC )
  add_definitions(
    -DPOCO_NO_AUTOMATIC_LIBS
    -DCRT_NONSTDC_NO_DEPRECATE
    -D_CRT_SECURE_NO_WARNINGS      # MS wanting you to change basically all standard C functions :(
    )

  #  4251 About exporting std classes
  #  4018 Comparing unsigned/signed ints
  #  4996 Deprecated functions
  add_definitions( "/wd4251 /wd4018 /wd4996 /nologo" )

  set (CMAKE_CXX_FLAGS             "${CMAKE_CXX_FLAGS} /EHsc ")
  set (CMAKE_SHARED_LINKER_FLAGS   "${CMAKE_SHARED_LINKER_FLAGS} ") #/FORCE:MULTIPLE")
endif()

add_subdirectory(rrplugins)

add_subdirectory(plugins)

add_subdirectory(wrappers)

