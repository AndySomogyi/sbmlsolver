cmake_minimum_required(VERSION 3.18)
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

# a macro for locating the vcpkg dependencies
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

if(BUILD_LLVM)
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
else()
    message(STATUS "NOT using LLVM")
endif(BUILD_LLVM)

if(BUILD_LEGACY_C)
    message(STATUS "building legacy C backend")
    add_definitions(-DBUILD_LEGACY_C)
else()
    message(STATUS "not building legacy C backend")
endif(BUILD_LEGACY_C)


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

if (${MSVC})
    add_definitions(
            -DPOCO_NO_AUTOMATIC_LIBS
            -DCRT_NONSTDC_NO_DEPRECATE
            -D_CRT_SECURE_NO_WARNINGS    #MS wanting you to changes basically all standard C functions :(
    )

    #  4251 About exporting std classes
    #  4018 Comparing unsigned/signed ints
    #  4996 Deprecated functions
    add_definitions("/wd4251 /wd4018 /wd4996 /nologo")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc ")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ") #/FORCE:MULTIPLE")

endif ()

if (${MINGW})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
endif ()

if (${BORLAND})
    add_definitions(
            -DUSE_PCH                   #Enable pre-compiled headers
            -H=${PROJECT_BINARY_DIR}/rr_pch.csm
            -w-8012                     #Comparing signed /unsigned
            -w-8057                     #Parameter never used
            -w-8004                     #'var' is assigned a value that is never used
    )
endif ()


if(UNIX)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
endif()

# Find vcpkg root if you can
#LocateVcpkg()

####################################################
#   Dependencies

# We begin by setting some paths. These are in predictable locations
# dependencies are embedded.

# Root of the dependencies
set(THIRD_PARTY_DIRECTORY "${CMAKE_SOURCE_DIR}/third_party")
CheckDirectoryExists("third_party directory" "${THIRD_PARTY_DIRECTORY}")

# clapack
set(CLAPACK_SOURCE_DIR ${THIRD_PARTY_DIRECTORY}/clapack3.2.1)
CheckDirectoryExists("clapack" "${CLAPACK_SOURCE_DIR}")
set(CLAPACK_INSTALL_PREFIX "${CLAPACK_SOURCE_DIR}/install-${CMAKE_CXX_COMPILER_ID}")

# we would need to refactor quite a lot to make this end at "include"
set(CLAPACK_INCLUDE_DIR "${CLAPACK_INSTALL_PREFIX}/include/clapack")

# libsbml (dependencies already taken care of)
set(LIBSBML_SOURCE_DIR "${THIRD_PARTY_DIRECTORY}/libSBML-5.18.1-experimental-Source")
CheckDirectoryExists("libsbml source" "${LIBSBML_SOURCE_DIR}")

set(LIBSBML_INSTALL_PREFIX "${LIBSBML_SOURCE_DIR}/install-${CMAKE_CXX_COMPILER_ID}")
set(LIBSBML_INCLUDE_DIR "${LIBSBML_INSTALL_PREFIX}/inlcude")
set(LIBSBML_STATIC_LIBRARY "${LIBSBML_INSTALL_PREFIX}/lib/libsbml-static${CMAKE_STATIC_LIBRARY_SUFFIX}")
set(LIBSBML_LIBRARY "${LIBSBML_INSTALL_PREFIX}/lib/libsbml${CMAKE_SHARED_LIBRARY_SUFFIX}")


# poco
set(POCO_SOURCE_DIR "${THIRD_PARTY_DIRECTORY}/poco_1.5.3")
CheckDirectoryExists("poco source" "${POCO_SOURCE_DIR}")
set(POCO_INSTALL_PREFIX "${POCO_SOURCE_DIR}/install-${CMAKE_CXX_COMPILER_ID}")
#set(POCO_INCLUDE_DIR "${POCO_INSTALL_PREFIX}/inlcude" PARENT_SCOPE)
#set(POCO_STATIC_LIBRARY "${POCO_INSTALL_PREFIX}/lib/POCO-static${CMAKE_STATIC_LIBRARY_SUFFIX}" PARENT_SCOPE)
#set(POCO_LIBRARY "${POCO_INSTALL_PREFIX}/lib/POCO${CMAKE_SHARED_LIBRARY_SUFFIX}" PARENT_SCOPE)


# Sundials
set(SUNDIALS_SOURCE_DIR "${THIRD_PARTY_DIRECTORY}/sundials")
CheckDirectoryExists("sundials source" "${SUNDIALS_SOURCE_DIR}")
set(SUNDIALS_INSTALL_PREFIX "${SUNDIALS_SOURCE_DIR}/install-${CMAKE_CXX_COMPILER_ID}")
#set(SUNDIALS_INCLUDE_DIR "${SUNDIALS_INSTALL_PREFIX}/inlcude" PARENT_SCOPE)
#set(SUNDIALS_STATIC_LIBRARY "${SUNDIALS_INSTALL_PREFIX}/lib/SUNDIALS-static${CMAKE_STATIC_LIBRARY_SUFFIX}" PARENT_SCOPE)
#set(SUNDIALS_LIBRARY "${SUNDIALS_INSTALL_PREFIX}/lib/SUNDIALS${CMAKE_SHARED_LIBRARY_SUFFIX}" PARENT_SCOPE)

# nleq1
set(NLEQ1_SOURCE_DIR "${THIRD_PARTY_DIRECTORY}/nleq1")
set(NLEQ1_INSTALL_PREFIX "${NLEQ1_SOURCE_DIR}/install-${CMAKE_CXX_COMPILER_ID}")

# nleq2
set(NLEQ2_SOURCE_DIR "${THIRD_PARTY_DIRECTORY}/nleq2")
set(NLEQ2_INSTALL_PREFIX "${NLEQ2_SOURCE_DIR}/install-${CMAKE_CXX_COMPILER_ID}")

# rr-libstruct
set(RR_LIBSTRUCT_DIR "${THIRD_PARTY_DIRECTORY}/rr-libstruct")
set(RR_LIBSTRUCT_INSTALL_PREFIX "${RR_LIBSTRUCT_DIR}/install-${CMAKE_CXX_COMPILER_ID}")

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

# print out the configuration summary
ConfigurationSummary()













