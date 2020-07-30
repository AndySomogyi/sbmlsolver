cmake_minimum_required(VERSION 3.17)
project(rr)

# libRoadRunner semantic versioning (http://semver.org/) - used to generate
# source files expose library version information

#########################################################################
# Version information and include modules

set(ROADRUNNER_VERSION_MAJOR 2)
set(ROADRUNNER_VERSION_MINOR 0)
set(ROADRUNNER_VERSION_PATCH 0)

set(ROADRUNNER_VERSION "${ROADRUNNER_VERSION_MAJOR}.${ROADRUNNER_VERSION_MINOR}.${ROADRUNNER_VERSION_PATCH}")

set(CMAKE_MACOSX_RPATH 1)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# defines a macro for finding vcpkg root
include(LocateVcpkg)

# defines a macro for printing out a configuration summary
# Technically this doesn't need to be a macro
# but it can get quite long so I like to put it out of sight.
include(ConfigurationSummary)

# a macro for isolating the code for finding dependencies
include(FindDependencies)

# function for locating llvm
include(FindLLVM)

# External project. Try to avoid
include(ExternalProject)

# Macro for determining whether to build or download the LLVM dependency.
# Put into a separate macro to increase readability in the main CMake script.
include(DownloadOrBuildLLVM)

# Macro for downloading binaries, when conditions are right (as determined by DownloadOrBuildLLVM)
include(DownloadLLVM601Binaries)

# Macro for building llvm6.0.1 from source.
include(BuildLLVMFromSource)

# Macro defining error for directory not found
include(CheckDirectoryExists)

# Macro for raising error when file not exist
include(CheckLibraryExists)

# macro for setting a variable with cross platform independent values
include(SetCrossPlatform)

#############################################################
#   Configuration Options
#

# Users should try to install LLVM6.0.1 on their own, outside
# the context of roadrunner. This is because LLVM is huge, and generally
# slows the performance of IDE's due to increased indexing.
# When users are providing their own LLVM6.0.1 installation, they give the root
# directory as argument to LLVM_INSTALL_PREFIX.
set(LLVM_INSTALL_PREFIX CACHE PATH "Location of LLVM Root directory, which contains bin, lib, include and share directories")


# should we build and install Testing tools?
option(BUILD_TEST_TOOLS "Build and install test tools" OFF)

# should we use LLVM ?
option(BUILD_LLVM "Build the LLVM back end" ON)


option(BUILD_LEGACY_C "Build the legacy C code generating backend (deprecated)")


# should we build the swig python wrapper?
option(BUILD_PYTHON "build the SWIG generated python wrapper" OFF)
set(PYTHON_SITE_DIR site-packages CACHE PATH "Path to Python site packages directory")
if (BUILD_PYTHON)
    set(PYTHON_PACKAGE_DEST_DIR ${PYTHON_SITE_DIR}/roadrunner)
endif ()

# should we build the swig Java wrapper?
option(BUILD_JAVA_INTERFACE "build the SWIG generated Java wrapper" OFF)
option(INSTALL_CXX_API "Generate C++ api" ON)
option(INSTALL_C_API "Generate C API" ON)
option(INSTALL_C_API_PYTHON "Package ctypes  Python wrapper for the C API" ON)
option(INSTALL_APPS "Build and install Apps" ON)
option(INSTALL_EXAMPLES "Build and install Examples" OFF)
option(INSTALL_STATIC_LIB "Install RoadRunner CXX static lib" OFF)
option(RR_BUILD_SHARED_CORE "Build RoadRunner Core Shared library" ON)
option(BUILD_TESTS "Build the SBML C API test suite" OFF)
option(INSTALL_SBML_MODELS "Install SBML Models" ON)
option(WITH_CONDA_BUILDER "Include files required to build the conda package" OFF)
option(USE_POCO_NET "Use poco net library" ON)
option(VERBOSE_CMAKE "set the VERBOSE_CMAKE flag to ON" OFF)

##################################################
#   Settings derived from options
#

if (BUILD_TESTS OR BUILD_JAVA_INTERFACE)
    enable_testing()
endif ()


mark_as_advanced(
        BUILD_LEGACY_C
        INSTALL_APPS
        INSTALL_CXX_API
        INSTALL_C_API
        INSTALL_C_API_PYTHON
        INSTALL_STATIC_LIB
        RR_USE_CXX14
)

# TODO: add some logic to automatically set based on compiler version
set(RR_USE_CXX14 TRUE CACHE BOOL "Set to TRUE to enable C++14 features")

# new clang on OSX and msvc use std namespace.
if (RR_USE_CXX14)
    option(USE_TR1_CXX_NS "Use the tr1 c++ namespace for swig smart pointer" OFF)
else ()
    option(USE_TR1_CXX_NS "Use the tr1 c++ namespace for swig smart pointer" ${CMAKE_COMPILER_IS_GNUCXX})
endif ()


set(CMAKE_BUILD_TYPE "Debug" CACHE STRING
        "Choose the build type. The options are: None (CMAKE_CXX_FLAGS or
     CMAKE_C_FLAGS are used), Debug, Release, RelWithDebInfo, MinSizeRel.")
if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING
            "Choose the build type. The options are: None (CMAKE_CXX_FLAGS or
  CMAKE_C_FLAGS are used), Debug, Release, RelWithDebInfo, MinSizeRel." FORCE)
endif ()

###################################################
#   Set some paths used in the build process
#

set(RR_GENERATED_HEADER_PATH ${CMAKE_CURRENT_BINARY_DIR}/source)
set(RR_ROADRUNNER_INSTALL_PATH ${CMAKE_INSTALL_PREFIX})
set(RR_ROADRUNNER_INSTALL_LIB_PATH ${RR_ROADRUNNER_INSTALL_PATH}/lib)
set(RR_SOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

#Setup so that roadrunner is always built as a dll and linked statically with 'as much as possible'
set(BUILD_SHARED_LIBS ON)

set(RR_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(RR_SOURCE_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(THIRD_PARTY_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/third_party)

set(exe_path ${PROJECT_BINARY_DIR}/bin)
set(lib_path ${PROJECT_BINARY_DIR}/lib)

if (WIN32)
    if (${MSVC})
        if (CMAKE_BUILD_TYPE MATCHES "Debug")
            set(exe_path ${exe_path}/Debug)
            set(lib_path ${lib_path}/Debug)
        else (CMAKE_BUILD_TYPE MATCHES "Debug")
            set(exe_path ${exe_path}/Release)
            set(lib_path ${lib_path}/Release)
        endif (CMAKE_BUILD_TYPE MATCHES "Debug")
    else (WIN32)
        # Linux
    endif (${MSVC})
endif (WIN32)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

if (BUILD_LLVM)
    #    message(STATUS "using LLVM, version ${LLVM_VERSION_MAJOR}.${LLVM_VERSION_MINOR}.${LLVM_VERSION_PATCH}")
    #
    #    message(STATUS "LLVM_LIBRARIES: ${LLVM_LIBRARIES}")
    #    message(STATUS "LLVM_FLAGS: ${LLVM_FLAGS}")
    #    message(STATUS "LLVM_INCLUDE_DIRS: ${LLVM_INCLUDE_DIRS}")
    #    message(STATUS "LLVM_LIBRARY_DIRS: ${LLVM_LIBRARY_DIRS}")
    add_definitions(-DBUILD_LLVM)
    # Don't use add_definitions for LLVM flags per http://stackoverflow.com/questions/13638408/cmake-override-compile-flags-for-single-files
    #     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LLVM_FLAGS}") # Set per-source
    #    include_directories(BEFORE ${LLVM_INCLUDE_DIRS})
else ()
    message(STATUS "NOT using LLVM")
endif (BUILD_LLVM)

if (BUILD_LEGACY_C)
    message(STATUS "building legacy C backend")
    add_definitions(-DBUILD_LEGACY_C)
else ()
    message(STATUS "not building legacy C backend")
endif (BUILD_LEGACY_C)


set(CMAKE_VERBOSE_MAKEFILE TRUE)

#####################################################
#    Apply project/compiler settings
#

# Set C++14 flags
if (RR_USE_CXX14)
    if (NOT MSVC)
        set(CMAKE_CXX_FLAGS "-std=c++14 ${CMAKE_CXX_FLAGS}")
    else ()
        message(STATUS "RR_USE_CXX14 enabled but has no effect on Microsoft compilers")
    endif ()
endif ()

# We always want LIBSBML_USE_CPP_NAMESPACE to be on
add_definitions(-DLIBSBML_USE_CPP_NAMESPACE=ON)

if (${MSVC})
    add_definitions(
            -DPOCO_NO_AUTOMATIC_LIBS
            -DCRT_NONSTDC_NO_DEPRECATE
            -D_CRT_SECURE_NO_WARNINGS    #MS wanting you to changes basically all standard C functions :(
    )

    # 4251 About exporting std classes
    # 4018 Comparing unsigned/signed ints
    # 4996 Deprecated functions
    # EHsc
    #   - https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-1-c4530?view=vs-2019
    #   - Fixes the compiler warning:
    #       - C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.26.28801\include\ostream(747):
    #         warning C4530: C++ exception handler used, but unwind semantics are not enabled. Specify /EHsc
    #
    add_definitions("/wd4251 /wd4018 /wd4996 /EHsc")

    #    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

endif ()

if (${MINGW})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
endif ()

#if (${BORLAND})
#    add_definitions(
#            -DUSE_PCH                   #Enable pre-compiled headers
#            -H=${PROJECT_BINARY_DIR}/rr_pch.csm
#            -w-8012                     #Comparing signed /unsigned
#            -w-8057                     #Parameter never used
#            -w-8004                     #'var' is assigned a value that is never used
#    )
#endif ()


if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
endif ()

# Find vcpkg root if you can
#LocateVcpkg()

####################################################
#   Dependencies

# We begin by setting some paths.

# We technically do not *need this to be a function, since it is only
#  used once. But it improves the readability of the main build script to
#  put the dependency locating code in a separate macro.
FindDependencies()

# handle the LLVM dependency
if (${LLVM_INSTALL_PREFIX})
    # This is the preferred way to handle the LLVM dependency.
    message(STATUS "Detected user defined installation of LLVM")
    #    set(LLVM_ROOT ${LLVM_INSTALL_PREFIX})
    # Do some checks then load
    message(STATUS "looking for llvm-config...:")
    find_program(LLVM_CONFIG
            NAMES llvm-config
            PATHS
            ${LLVM_INSTALL_PREFIX}/bin/llvm-config
            ${LLVM_INSTALL_PREFIX}/bin/llvm-config.lib
            )

    if (NOT EXISTS ${LLVM_CONFIG})
        message(FATAL_ERROR "Could not find llvm-config executable at ${LLVM_CONFIG}")
    endif ()
    message(STATUS "llvm-config found at \"${LLVM_CONFIG}\"")

else (${LLVM_INSTALL_PREFIX})
    # But we can fall back and download binaries or build from source as well

    # Set the root of llvm to be in the third_party directory. Note
    # that we need to separate different compilers
    set(LLVM_ROOT "${THIRD_PARTY_DIRECTORY}/LLVM6.0.1/${CMAKE_CXX_COMPILER_ID}")

    # set llvm source directory. This is only used when we are in LLVM build mode
    set(LLVM_SOURCE_DIR "${LLVM_ROOT}/LLVM")

    # set a place to put the binaries
    SetCrossPlatform(LLVM_INSTALL_PREFIX
            "${LLVM_ROOT}/llvm6.0.1-msvc"
            "${LLVM_ROOT}/llvm6.0.1-ubuntu"
            "${LLVM_ROOT}/llvm6.0.1-mac"
            )

    ## first try to find an existing LLVM version
    find_package(LLVM)

    # if it was found, validate the version
    if (LLVM_FOUND)
        if (NOT ${LLVM_VERSION} STREQUAL "6.0.1")
            message(FATAL_ERROR "Wrong version of LLVM found. Expected 6.0.1 but found ${LLVM_VERSION}")
        endif ()
    else ()
        # Otherwise we download binaries or build from source

        # defines two boolean variables
        #   - BUILD_LLVM
        #   - DOWNLOAD_LLVM_BINARIES
        # They are mutually exclusive and determine the flow of the next step
        DownloadOrBuildLLVM()

        # If conditions are met we can use precompiled binaries
        if (${DOWNLOAD_LLVM_BINARIES})
            DownloadLLVM601Binaries()
        endif ()

        # if not we can build LLVM from source, which will take a while
        if (BUILD_LLVM)
            BuildLLVMFromSource()
        endif (BUILD_LLVM)

        # Now we try again to find llvm
        #        find_package(LLVM)

    endif (LLVM_FOUND)
endif (${LLVM_INSTALL_PREFIX})


# add the third_party directory as dependencies
add_subdirectory(third_party)

##########################################################
# Add roadrunner source directories to the build

# We always have to build the roadrunner core, everything depends on this
add_subdirectory(source)

# install the rr_support if building legacy c
if(BUILD_LEGACY_C)
    add_subdirectory(rr_support)
endif(BUILD_LEGACY_C)

##########################################################
# Build tests


if(BUILD_TESTS AND BUILD_TEST_TOOLS)
#    message(STATUS "building tests")
#    file (GLOB TEST_FILES "testing/*" )
#
#    file (GLOB PYTHON_TEST_FILES "wrappers/Python/roadrunner/testing/*" )
#    list( APPEND TEST_FILES ${PYTHON_TEST_FILES} )
#
#    file(GLOB PYTHON_TEST_DATA "${CMAKE_CURRENT_SOURCE_DIR}/testing/*.rrtest")
#
#    file(GLOB TEST_DATA_XML "${CMAKE_CURRENT_SOURCE_DIR}/testing/*.xml")
#
#    foreach( f ${TEST_FILES} )
#        if(NOT IS_DIRECTORY ${f})
#            get_filename_component( FILE_NAME ${f} NAME )
#            configure_file( ${f} testing/${FILE_NAME} COPYONLY )
#            configure_file( ${f} lib/site-packages/roadrunner/testing/${FILE_NAME} COPYONLY )
#        endif()
#    endforeach()
#
#    foreach( f ${PYTHON_TEST_DATA} )
#        if(NOT IS_DIRECTORY ${f})
#            get_filename_component( FILE_NAME ${f} NAME )
#            configure_file( ${f} lib/site-packages/roadrunner/testing/test_data/${FILE_NAME} COPYONLY )
#        endif()
#    endforeach()
#
#    foreach( f ${TEST_DATA_XML} )
#        if(NOT IS_DIRECTORY ${f})
#            get_filename_component( FILE_NAME ${f} NAME )
#            configure_file( ${f} lib/site-packages/roadrunner/testing/test_data/${FILE_NAME} COPYONLY )
#        endif()
#    endforeach()
#
#    add_subdirectory(testing)
#    add_subdirectory(autotest)
else()
    message(STATUS "NOT building tests")
endif()


##################################################
#   Build wrappers etc


#if(INSTALL_CXX_API)
#    if(INSTALL_APPS)
#        add_subdirectory(apps)
#    endif()
#
#    if(INSTALL_EXAMPLES)
#        add_subdirectory(examples)
#    endif()
#endif(INSTALL_CXX_API)
#
#add_subdirectory(wrappers)

# print out the configuration summary
ConfigurationSummary()













