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
set(CLAPACK_INCLUDE_DIR "${CLAPACK_SOURCE_DIR}/include")
CheckDirectoryExists("clapack source" "${CLAPACK_SOURCE_DIR}")

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


# handle the LLVM dependency
if (${LLVM_INSTALL_PREFIX})
    # This is the preferred way to handle the LLVM dependency.
    message(STATUS "Detected user defined installation of LLVM")
#    set(LLVM_ROOT ${LLVM_INSTALL_PREFIX})

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

#
## add the third_party directory as dependencies
#add_subdirectory(third_party)
#
#
## TODO: add some logic to automatically set based on compiler version
#set(RR_USE_CXX14 TRUE CACHE BOOL "Set to TRUE to enable C++14 features")
#

# print out the configuration summary
ConfigurationSummary()













