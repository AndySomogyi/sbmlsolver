cmake_minimum_required(VERSION 3.17)
project(rr)

# libRoadRunner semantic versioning (http://semver.org/) - used to generate
# source files expose library version information

#########################################################################
# Version information and include modules

set(ROADRUNNER_VERSION_MAJOR 2)
set(ROADRUNNER_VERSION_MINOR 0)
set(ROADRUNNER_VERSION_PATCH 1)

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

# convenience function for querying presently defined cmake variables
include(QueryCMakeVariables)

# Convenience functions for setting MSVC runtime
include(SetTargetMSVCRuntimeStatic)
include(SetTargetMSVCRuntimeDll)
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
option(CMAKE_VERBOSE_MAKEFILE "verbose output" ON)

set(CMAKE_STANDARD 14 CACHE STRING "set the cmake standard to use")

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
set(BUILD_SHARED_LIBS            ON)
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
    # todo needed?
    add_definitions(-DBUILD_LLVM)
else ()
    message(STATUS "NOT using LLVM")
endif (BUILD_LLVM)

if (BUILD_LEGACY_C)
    message(STATUS "building legacy C backend")
    add_definitions(-DBUILD_LEGACY_C)
else ()
    message(STATUS "not building legacy C backend")
endif (BUILD_LEGACY_C)

#####################################################
#    Apply project/compiler settings
#
# Set C++14 flags
set(CMAKE_CXX_STANDARD ${CMAKE_STANDARD})

set(CMAKE_VERBOSE_MAKEFILE TRUE)

if (${MSVC})
    # Here we set all flags that are needed for dependency packages.
    # Since we link to everything statically, we need these flags.
    # Individual targets will need RR_STATIC or RR_EXPORT.
    add_definitions(
            -DLIBSBML_USE_CPP_NAMESPACE
            -DLIBSBML_STATIC
            -DLIBLAX_STATIC
            -DSTATIC_LIBSTRUCT
            -DSTATIC_PUGI
            -DSTATIC_NLEQ1
            -DSTATIC_NLEQ2
            -DPOCO_STATIC
            -DPOCO_NO_AUTOMATIC_LIBS
            -DCRT_NONSTDC_NO_DEPRECATE
            -D_CRT_SECURE_NO_WARNINGS    #MS wanting you to changes basically all standard C functions :(
            -D_WIN32 # used as switch in zlib macro
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

####################################################
#   Dependencies

# add the third_party directory as dependencies
add_subdirectory(third_party)

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

    if (NOT EXISTS ${LLVM_INSTALL_PREFIX}/share/llvm/cmake)
        message(FATAL_ERROR "${LLVM_INSTALL_PREFIX}/share/llvm/cmake" not found)
    endif ()
    message(STATUS "llvm-config found at \"${LLVM_CONFIG}\"")

    # Use the propper LLVM supplied CMake files.
    # prepend the cmake module path with the LLVM modules
    set(LLVM_MODULE_PATH "${LLVM_INSTALL_PREFIX}/lib/llvm/cmake")
    if (NOT EXISTS "${LLVM_MODULE_PATH}")
        message(FATAL_ERROR "LLVM module path expected at ${LLVM_MODULE_PATH} but it was not found")
    endif ()
    set(CMAKE_MODULE_PATH "${LLVM_MODULE_PATH} ${CMAKE_MODULE_PATH}")

    include(LLVMConfig)

    # we're building a JIT compiler with support for binary code (no interpreter):
    # this sets the LLVM_LIBRARIES var to be the list of required LLVM libs
    # to link with.
    llvm_map_components_to_libraries(LLVM_LIBRARIES jit native)

endif ()
# This code should not be deleted as it can be revived. Commented out for now (Ciaran welsh)
#else (${LLVM_INSTALL_PREFIX})
#    # But we can fall back and download binaries or build from source as well
#
#    # Set the root of llvm to be in the third_party directory. Note
#    # that we need to separate different compilers
#    set(LLVM_ROOT "${THIRD_PARTY_DIRECTORY}/LLVM6.0.1/${CMAKE_CXX_COMPILER_ID}")
#
#    # set llvm source directory. This is only used when we are in LLVM build mode
#    set(LLVM_SOURCE_DIR "${LLVM_ROOT}/LLVM")
#
#    # set a place to put the binaries
#    SetCrossPlatform(LLVM_INSTALL_PREFIX
#            "${LLVM_ROOT}/llvm6.0.1-msvc"
#            "${LLVM_ROOT}/llvm6.0.1-ubuntu"
#            "${LLVM_ROOT}/llvm6.0.1-mac"
#            )
#
#    ## first try to find an existing LLVM version
#    find_package(LLVM)
#
#    # if it was found, validate the version
#    if (LLVM_FOUND)
#        if (NOT ${LLVM_VERSION} STREQUAL "6.0.1")
#            message(FATAL_ERROR "Wrong version of LLVM found. Expected 6.0.1 but found ${LLVM_VERSION}")
#        endif ()
#    else ()
#        # Otherwise we download binaries or build from source
#
#        # defines two boolean variables
#        #   - BUILD_LLVM
#        #   - DOWNLOAD_LLVM_BINARIES
#        # They are mutually exclusive and determine the flow of the next step
#        DownloadOrBuildLLVM()
#
#        # If conditions are met we can use precompiled binaries
#        if (${DOWNLOAD_LLVM_BINARIES})
#            DownloadLLVM601Binaries()
#        endif ()
#
#        # if not we can build LLVM from source, which will take a while
#        if (BUILD_LLVM)
#            BuildLLVMFromSource()
#        endif (BUILD_LLVM)
#
#        # Now we try again to find llvm
#        #        find_package(LLVM)
#
#    endif (LLVM_FOUND)
#endif (${LLVM_INSTALL_PREFIX})

# Collect our include directories into a convenient variable
set(DEPENDENCY_INCLUDE_DIRS
        "${NLEQ1_INCLUDE_DIR}"
        "${NLEQ2_INCLUDE_DIR}"
        "${CLAPACK_INCLUDE_DIR}"
        "${LIBSBML_DEPS_INCLUDE_DIR}"
        "${LIBSBML_INCLUDE_DIR}"
        "${RR_STRUCT_INCLUDE_DIR}"
        "${POCO_INCLUDE_DIR}"
        "${SUNDIALS_INCLUDE_DIR}"
        "${UNITTEST_INCLUDE_DIR}"
        )
if(${MINGW})
    SET (CMAKE_CXX_FLAGS             "${CMAKE_CXX_FLAGS} -std=gnu++0x")
endif()

#if(${BORLAND})
#    link_directories(${THIRD_PARTY_INSTALL_FOLDER}/lib)
#add_definitions(
#    -DUSE_PCH                   #Enable pre-compiled headers
#    -H=${PROJECT_BINARY_DIR}/rr_pch.csm
#    -w-8012                     #Comparing signed /unsigned
#    -w-8057                     #Parameter never used
#    -w-8004                     #'var' is assigned a value that is never used
#    )
#endif()


# Collect all dependency targets into a single list
# Note the exclusion of LLVM libraries, which are not targets
set(LINK_TARGETS
        "blas"
        "f2c"
        "lapack"
        "sbml-static"
        "libiconv-static"
        "libLZMA-static"
        "libxml2-static"
        "zlibstatic"
        "nleq1-static"
        "nleq2-static"
        "PocoFoundation"
        "PocoNet"
        "PocoUtil"
        "PocoXML"
        "PocoZip"
        "PocoJSON"
        "rr-libstruct-static"
        "sundials_cvode_static"
        "sundials_cvodes_static"
        "sundials_ida_static"
        "sundials_idas_static"
        "sundials_kinsol_static"
        "sundials_nvecserial_static"
        )

# note: unit test will be replaced by gtest eventually
set(TEST_TARGETS "unit_test-static" "gtest" "gtest_main")

# create new custom target so we can build all-dependencies and
#  build all deps with single command
add_custom_target(all-dependencies)
add_dependencies(all-dependencies ${LINK_TARGETS})

# now add LLVM.
set(LINK_LIBRARIES "${LINK_TARGETS}" "${LLVM_LIBRARIES}")

##########################################################
# Add roadrunner source directories to the build

# We always have to build the roadrunner core, everything depends on this
add_subdirectory(source)

##########################################################
# Build tests

#if (BUILD_TESTS AND BUILD_TEST_TOOLS)
#
#    set(GOOGLETEST_SOURCE_DIR third_party/googletest)
#    set(GOOGLETEST_INCLUDE_DIR ${GOOGLETEST_SOURCE_DIR}/googletest)
#    set(GOOGLEMOCK_INCLUDE_DIR ${GOOGLETEST_SOURCE_DIR}/googlemock)
#
#    add_subdirectory(${GOOGLETEST_SOURCE_DIR})
#
#    add_subdirectory(tests)
#
#    #    QueryCMakeVariables()
#
#    # I really have no idea what this code is doing. (cw)
#    # This code comes with no comments, so i'll add some as I'm
#    # trying to figure out what its doing
#    message(STATUS "building tests")
#
#    # Collect a list of all files inside "testing" folder.
#    # This includes CMakeLists.txt for some reason.
#    file(GLOB TEST_FILES "testing/*")
#
#    MESSAGE(STATUS "TEST_FILES ${TEST_FILES}")
#
#    # Collect the set of Python test files.
#    file(GLOB PYTHON_TEST_FILES "wrappers/Python/roadrunner/testing/*")
#
#    MESSAGE(STATUS "PYTHON_TEST_FILES ${PYTHON_TEST_FILES}")
#
#    # and combine the two list of test files
#    list(APPEND TEST_FILES ${PYTHON_TEST_FILES})
#
#    # Collect all python and xml test data
#    file(GLOB PYTHON_TEST_DATA "${CMAKE_CURRENT_SOURCE_DIR}/testing/*.rrtest")
#
#    file(GLOB TEST_DATA_XML "${CMAKE_CURRENT_SOURCE_DIR}/testing/*.xml")
#
#    # Oh, I think I see what has happened here.
#    # This code makes a copy of each test fle, both python and otherwise
#    foreach (f ${TEST_FILES})
#        if (NOT IS_DIRECTORY ${f})
#            get_filename_component(FILE_NAME ${f} NAME)
#            #            message(STATUS ${FILE_NAME})
#            # This command copies into the main testing directory under project root.
#            # Dont know why though.
#            # Could be that somebody just committed these files and they havent been deleted.
#            configure_file(${f} testing/${FILE_NAME} COPYONLY)
#
#            # Uncertain why these have been copied here. Presumably its only the Python files
#            # That need to go into a site packages directory, though still no idea
#            # Why a site packages directory is needed. Also, I still havent found this
#            configure_file(${f} lib/site-packages/roadrunner/testing/${FILE_NAME} COPYONLY)
#        endif ()
#    endforeach ()
#
#    foreach (f ${PYTHON_TEST_DATA})
#        if (NOT IS_DIRECTORY ${f})
#            get_filename_component(FILE_NAME ${f} NAME)
#            configure_file(${f} lib/site-packages/roadrunner/testing/test_data/${FILE_NAME} COPYONLY)
#        endif ()
#    endforeach ()
#
#    foreach (f ${TEST_DATA_XML})
#        if (NOT IS_DIRECTORY ${f})
#            get_filename_component(FILE_NAME ${f} NAME)
#            configure_file(${f} lib/site-packages/roadrunner/testing/test_data/${FILE_NAME} COPYONLY)
#        endif ()
#    endforeach ()
#
#    # I don't understand. The testing directory doesn't build anything???
#    add_subdirectory(testing)
#    add_subdirectory(autotest)
#    add_subdirectory(source/testing)
#    add_subdirectory(source/llvm_testing)
#else ()
#    message(STATUS "NOT building tests")
#endif ()
#
#
###################################################
##   Build wrappers etc
#
##if (INSTALL_CXX_API)
##    if (INSTALL_APPS)
##        add_subdirectory(apps)
##    endif ()
##
##    if (INSTALL_EXAMPLES)
##        add_subdirectory(examples)
##    endif ()
##endif (INSTALL_CXX_API)
##
##add_subdirectory(wrappers)
#
## print out the configuration summary
## note configure summary not current, uncomment once its fixed
##ConfigurationSummary()
#
### make or copy installers into root directory
##add_subdirectory(installer)
##
###=== ThirdParties
##if(${BORLAND})
##    set(CG_RUNTIMES        cc32110MT.dll)
##    foreach(runtime ${CG_RUNTIMES})
##        install (FILES
##            ${THIRD_PARTY_INSTALL_FOLDER}/cg/xe/${runtime}
##            DESTINATION bin
##            COMPONENT rr_core)
##    endforeach()
##endif()
#
#configure_file(VERSION.txt.in VERSION.txt @ONLY)
#
#install(
#    FILES
#    NOTICE.txt
#    ${CMAKE_CURRENT_BINARY_DIR}/VERSION.txt
#    NEWS.txt
#    DESTINATION .
#)
#


