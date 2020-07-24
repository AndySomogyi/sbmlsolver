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

#############################################################
#   Configuration Options
#


# Find vcpkg root if you can
LocateVcpkg()

####################################################
#   Dependencies

# Root of the dependencies
set(THIRD_PARTY_DIRECTORY "${CMAKE_SOURCE_DIR}/third_party")

message(STATUS "VCPKG_LIB_DIR ${VCPKG_LIB_DIR}")

# First we handle the LLVM dependency

# Set the root of llvm to be in the third_party directory. Note
# that we need to separate different compilers
set(LLVM_ROOT "${THIRD_PARTY_DIRECTORY}/LLVM6.0.1/${CMAKE_CXX_COMPILER_ID}")

# set llvm source directory. This is only used when we are in LLVM build mode
set(LLVM_SOURCE_DIR ${LLVM_ROOT}/LLVM)

# set a place to put the binaries
set(LLVM_INSTALL_DIR ${LLVM_ROOT}/llvm6.0.1-ubuntu)

# first try to find an existing LLVM version
find_package(LLVM)

# if it was found, validate the version
if (LLVM_FOUND)
    set(LLVM_VERSION "${LLVM_VERSION_MAJOR}.${LLVM_VERSION_MINOR}.${LLVM_VERSION_PATCH}")
    if (NOT ${LLVM_VERSION} STREQUAL "6.0.1")
        message(FATAL_ERROR "Wrong version of LLVM found. Expected 6.0.1 but found \
${LLVM_VERSION}")
    endif ()
else ()
    # Otherwise we download binaries or sources

    # defines two boolean variables
    #   - BUILD_LLVM
    #   - DOWNLOAD_LLVM_BINARIES
    # They are mutually exclusive and determine the flow of the next step
    #    DownloadOrBuildLLVM()

    # temporary: for testing
    set(BUILD_LLVM ON)
    set(DOWNLOAD_LLVM_BINARIES OFF)
    # Whatever I do here with llvm, it needs to be a target
    if (${DOWNLOAD_LLVM_BINARIES})
        DownloadLLVM601Binaries()
    endif ()

    # if not we can build LLVM from source, which will take a while
    if (BUILD_LLVM)
        set(LLVM_SOURCE_URL "https://github.com/CiaranWelsh/LLVM6.0.1/archive/v0.0.1.zip")
        set(LLVM_SOURCE_ZIP "${LLVM_ROOT}/llvm6.0.1-source.zip")
        set(LLVM_LIBRARIES
               "LLVMAnalysis"
               "LLVMAsmPrinter"
               "LLVMBinaryFormat"
               "LLVMBitReader"
               "LLVMBitWriter"
               "LLVMCodeGen"
               "LLVMCore"
               "LLVMDebugInfoCodeView"
               "LLVMDebugInfoMSF"
               "LLVMDemangle"
               "LLVMExecutionEngine"
               "LLVMGlobalISel"
               "LLVMInstCombine"
               "LLVMMC"
               "LLVMMCDisassembler"
               "LLVMMCJIT"
               "LLVMMCParser"
               "LLVMObject"
               "LLVMProfileData"
               "LLVMRuntimeDyld"
               "LLVMScalarOpts"
               "LLVMSelectionDAG"
               "LLVMSupport"
               "LLVMTarget"
               "LLVMTransformUtils"
               "LLVMX86AsmParser"
               "LLVMX86AsmPrinter"
               "LLVMX86CodeGen"
               "LLVMX86Desc"
               "LLVMX86Info"
               "LLVMX86Utils"
                )

        set(LLVM_SOURCE_DIR "${LLVM_ROOT}/llvm6.0.1-source")
        set(LLVM_BINARY_DIR "${LLVM_ROOT}/llvm6.0.1-source")
        ExternalProject_Add(LLVM
                PREFIX ${LLVM_SOURCE_DIR}
                INSTALL_DIR ${LLVM_INSTALL_DIR}
                URL ${LLVM_SOURCE_URL}
                CMAKE_ARGS
                -DCMAKE_INSTALL_PREFIX=${LLVM_INSTALL_DIR}
                --target ${LLVM_LIBRARIES}
                )

    endif (BUILD_LLVM)

endif ()

# first find dependencies that we can get from vcpkg
#FindDependencies()

# add the third_party directory as dependencies
#add_subdirectory(third_party)


# print out the configuration summary
ConfigurationSummary()













