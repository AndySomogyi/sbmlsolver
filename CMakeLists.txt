cmake_minimum_required(VERSION 3.17)

# libRoadRunner semantic versioning (http://semver.org/) - used to generate
# source files expose library version information

#########################################################################
# Version information and include modules

set(ROADRUNNER_VERSION_MAJOR 2)
set(ROADRUNNER_VERSION_MINOR 0)
set(ROADRUNNER_VERSION_PATCH 1)

set(ROADRUNNER_VERSION "${ROADRUNNER_VERSION_MAJOR}.${ROADRUNNER_VERSION_MINOR}.${ROADRUNNER_VERSION_PATCH}")
project(
        roadrunner
        DESCRIPTION "roadrunner project"
        VERSION ${ROADRUNNER_VERSION}
        LANGUAGES CXX
)

set(CMAKE_MACOSX_RPATH 1)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# defines a macro for printing out a configuration summary
include(ConfigurationSummary)

# External project. Try to avoid
include(ExternalProject)

# macro for setting a variable with cross platform independent values
include(SetCrossPlatform)

# convenience function for querying presently defined cmake variables
include(QueryCMakeVariables) # debbugging

# GNU standard install locations
include(GNUInstallDirs)

# generate automatic export headers
include(GenerateExportHeader)

# Custom macro for containing the git related stuff.
include(WriteGitInfo)
write_git_info() # write the git branch and commit hash to console

#############################################################
#   Configuration Options
#

# any of these directories are accepted as default directories.
# this supports multi platform builds on a single system
if (${WIN32})
    set(RR_DEPENDENCIES_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/dependencies-windows"
            CACHE PATH "Path to dependency package"
            )
elseif (${UNIX})
    set(RR_DEPENDENCIES_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/dependencies-unix"
            CACHE PATH "Path to dependency package"
            )
elseif (${APPLE})
    set(RR_DEPENDENCIES_INSTALL_PREFIX "{CMAKE_CURRENT_SOURCE_DIR}/dependencies-mac"
            CACHE PATH "Path to dependency package"
            )
else ()
    message(FATAL_ERROR "Unknown operating system")
endif ()

# This variable provides users a way of providing their own LLVM installation.
# The two options are: 1) build llvm along with the other dependencies or
# 2) build the other dependencies and llvm separate. If the latter is chosen,
# users will need to provide their own argument to LLVM_INSTALL_PREFIX
set(LLVM_INSTALL_PREFIX ${RR_DEPENDENCIES_INSTALL_PREFIX} CACHE PATH "Location of LLVM Root directory, which contains bin, lib, include and share directories")
set(LLVM_ROOT_DIR "${LLVM_ROOT_DIR}")
# should we build and install Testing tools?
option(BUILD_TEST_TOOLS "Build and install test tools" OFF)

# should we use LLVM ?
option(BUILD_LLVM "Build the LLVM back end" ON)


# todo consider when to remove the legacy backend?
option(BUILD_LEGACY_C "Build the legacy C code generating backend (deprecated)")


# should we build the swig python wrapper?
option(BUILD_PYTHON "build the SWIG generated python wrapper" OFF)
set(PYTHON_SITE_DIR site-packages CACHE PATH "Path to Python site packages directory")
if (BUILD_PYTHON)
    set(PYTHON_PACKAGE_DEST_DIR ${PYTHON_SITE_DIR}/roadrunner)
endif ()

#TODO remove options that arent really options

option(BUILD_JAVA_INTERFACE "build the SWIG generated Java wrapper" OFF)
option(INSTALL_CXX_API "Generate C++ api" ON)
option(INSTALL_C_API "Generate C API" ON)
option(INSTALL_C_API_PYTHON "Package ctypes  Python wrapper for the C API" ON)
option(INSTALL_APPS "Build and install Apps" ON)
option(INSTALL_EXAMPLES "Build and install Examples" OFF)
option(INSTALL_STATIC_LIB "Install RoadRunner CXX static lib" OFF)
option(RR_BUILD_SHARED_CORE "Build RoadRunner Core Shared library" ON)
option(BUILD_TESTS "Build the SBML C API test suite" OFF)
option(INSTALL_SBML_MODELS "Install SBML Models" OFF)
option(WITH_CONDA_BUILDER "Include files required to build the conda package" OFF)
option(USE_POCO_NET "Use poco net library" ON)
option(VERBOSE_CMAKE "set the VERBOSE_CMAKE flag to ON" OFF)
option(CMAKE_VERBOSE_MAKEFILE "verbose output" ON)
option(BUILD_PACKAGING "Build binary distributions" OFF)
option(WITH_ADDRESS_SANITIZER "Build roadrunner with memory sanitizer tool" OFF)

# teplugin options

## Feature selection
option(TLP_BUILD_SHARED_LIB "TLP_BUILD_SHARED_LIB" ON)
option(TLP_BUILD_STATIC_LIB "TLP_BUILD_STATIC_LIB" ON)
option(TLP_BUILD_PLUGINS_C_API "Build Plugins C API" ON)
option(TLP_BUILD_PLUGINS_PYTHON_API "Build Plugins Python API" ON)
option(RR_PLUGINS_INSTALL_TO_RR_SITE_PACKAGES "Install libs to roadrunner site-packages directory" ON)
set(RRP_PLUGINS_DIR site-packages/roadrunner CACHE PATH "Destination for plugins")
# Plugin selection: Released
option(TLP_BUILD_TEST_MODEL_PLUGIN   "Build the Test Model Plugin"  OFF)
option(TLP_BUILD_ADD_NOISE_PLUGIN  "Build the Add Noise Plugin"  OFF)
option(TLP_BUILD_CHISQUARE_PLUGIN  "Build the ChiSquare Plugin"  OFF)
#set (TLP_BUILD_LEVENBERG_MARQUARDT_PLUGIN ON CACHE BOOL "Build the Levenberg-Marquardt Plugin")
#set (TLP_BUILD_NELDER_MEAD_PLUGIN ON CACHE BOOL "Build the Nelder-Mead Plugin")
option(TLP_BUILD_AUTO2000_PLUGIN "Build the Auto 2000 Plugin" ON )
option(TLP_BUILD_HELLO_PLUGIN "Build the Demo Plugin" ON)


set(CXX_STANDARD 14 CACHE STRING "set the cmake standard to use")

##################################################
#   Settings derived from options
#

# Would be nice to set these options but I tried it and we cannot without
# breaking the build
#set(CMAKE_CXX_VISIBILITY_PRESET hidden)
#set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

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
# todo: turn on C++17 and see if it errors.
#   - it does. Therefore todo: fix errors with c++17
set(RR_USE_CXX14 TRUE CACHE BOOL "Set to TRUE to enable C++14 features")

# new clang on OSX and msvc use std namespace.
if (RR_USE_CXX14)
    option(USE_TR1_CXX_NS "Use the tr1 c++ namespace for swig smart pointer" OFF)
else ()
    option(USE_TR1_CXX_NS "Use the tr1 c++ namespace for swig smart pointer" ${CMAKE_COMPILER_IS_GNUCXX})
endif ()


set(CMAKE_BUILD_TYPE "Release" CACHE STRING
        "Choose the build type. The options are: None (CMAKE_CXX_FLAGS or
     CMAKE_C_FLAGS are used), Debug, Release, RelWithDebInfo, MinSizeRel.")

if (WITH_ADDRESS_SANITIZER)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif ()


#Setup so that roadrunner is always built as a dll and linked statically with 'as much as possible'
option(BUILD_SHARED_LIBS "Turns on building shared libraries in addition to static libraries which are always build" ON)

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


# Set C++14 flags
set(CMAKE_CXX_STANDARD ${CXX_STANDARD})

# Enable cmake printing out commands
set(CMAKE_VERBOSE_MAKEFILE TRUE)

if (${MSVC})
    # Here we set all flags that are needed for dependency packages.
    # Since we link to everything statically, we need these flags.
    # Individual targets will need RR_STATIC or RR_EXPORT but otherwise
    # these flags are set globally.
    # todo consider whether this is desirable behaviour in the scenario that
    #   roadrunner is being used as a embedded package
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
            -D_CRT_SECURE_NO_WARNINGS    # MS wanting you to changes basically all standard C functions :(
            -D_WIN32                     # used as switch in zlib macro
    )

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

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
endif ()

set(CMAKE_DEBUG_POSTFIX "d")

####################################################
#   Dependencies

if (NOT EXISTS ${RR_DEPENDENCIES_INSTALL_PREFIX})
    message(FATAL_ERROR "The path given to -DRR_DEPENDENCIES_INSTALL_PREFIX (\"${RR_DEPENDENCIES_INSTALL_PREFIX}\") does not exist. Please \
build the dependency package. If you set
    -DCMAKE_INSTALL_PREFIX=/path/to/roadrunner/root/dependencies
they will be found automatically. If not, you will need to pass in
    -DRR_DEPENDENCIES_INSTALL_PREFIX=\"/full/path/to/dependency/install/tree\"
to the roadrunner cmake command.")
endif ()
set(DEPENDENCY_INCLUDE_DIR "${RR_DEPENDENCIES_INSTALL_PREFIX}/include")

if (NOT EXISTS ${DEPENDENCY_INCLUDE_DIR})
    message(FATAL_ERROR "Cannot find the dependency include directory in your \
dependency install tree. Please ensure the path you have given to -DRR_DEPENDENCIES_INSTALL_PREFIX \
exists and is the full path to the installed dependency tree.")
endif ()

set(DEPENDENCY_CMAKE_CONFIG_PATHS "${RR_DEPENDENCIES_INSTALL_PREFIX}/lib/cmake")
message(STATUS "DEPENDENCY_CMAKE_CONFIG_PATHS ${DEPENDENCY_CMAKE_CONFIG_PATHS}")
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "${DEPENDENCY_CMAKE_CONFIG_PATHS}")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${DEPENDENCY_CMAKE_CONFIG_PATHS}")

find_package(Threads) # for libxml2, FindThreads.cmake is shipped with cmake
find_package(LibLZMA) # for libxml2, LibLZMA.cmake is shipped with cmake
find_package(zlib CONFIG REQUIRED)
find_package(bzip2 CONFIG REQUIRED)
find_package(iconv CONFIG REQUIRED)
find_package(LibXml2 CONFIG REQUIRED)
find_package(sbml-static CONFIG REQUIRED)
find_package(rr-libstruct CONFIG REQUIRED)
find_package(clapack CONFIG REQUIRED)
find_package(nleq1 CONFIG REQUIRED)
find_package(nleq2 CONFIG REQUIRED)
find_package(PocoFoundation CONFIG REQUIRED)
find_package(PocoNet CONFIG REQUIRED)
find_package(Sundials CONFIG REQUIRED)
find_package(LLVM REQUIRED)

# install dependencies so clients can use roadrunner from c++/cmake
install(DIRECTORY ${RR_DEPENDENCIES_INSTALL_PREFIX}/lib DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY ${RR_DEPENDENCIES_INSTALL_PREFIX}/bin DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY ${RR_DEPENDENCIES_INSTALL_PREFIX}/include DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY ${RR_DEPENDENCIES_INSTALL_PREFIX}/share DESTINATION ${CMAKE_INSTALL_PREFIX})

# install a cmake script for consumer libraries to easily import roadrunner targets
install(FILES cmake/ImportRoadrunnerAndDependencies.cmake DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake")

if (${MINGW})
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
endif ()

if(${BORLAND})
    link_directories(${THIRD_PARTY_INSTALL_FOLDER}/lib)
add_definitions(
    -DUSE_PCH                   #Enable pre-compiled headers
    -H=${PROJECT_BINARY_DIR}/rr_pch.csm
    -w-8012                     #Comparing signed /unsigned
    -w-8057                     #Parameter never used
    -w-8004                     #'var' is assigned a value that is never used
    )
endif()



##########################################################
# Building
#

# We always have to build the roadrunner core, everything depends on this
add_subdirectory(source)


##########################################################
# Tests
#

# Empty space ready for when tests arrive.

#
###########################################################
## API's

add_subdirectory(wrappers)


##########################################################
# Wrappers
#

#add_subdirectory(docs)


##########################################################
# Packaging
#

if (${BUILD_PACKAGING})
    #todo raise question of deleting the "installer" folder (cw)
    # it doesn't look that useful and we now have the contents of
    # packaging that will automate production of binary releases.
    add_subdirectory(packaging)
endif ()


##########################################################
# Configuration summary
#

if(BUILD_TESTS AND BUILD_TEST_TOOLS)
    message(STATUS "building tests")
    file (GLOB TEST_FILES "testing/*" )

    file (GLOB PYTHON_TEST_FILES "wrappers/Python/roadrunner/testing/*" )
    list( APPEND TEST_FILES ${PYTHON_TEST_FILES} )

    file(GLOB PYTHON_TEST_DATA "${CMAKE_CURRENT_SOURCE_DIR}/testing/*.rrtest")

    file(GLOB TEST_DATA_XML "${CMAKE_CURRENT_SOURCE_DIR}/testing/*.xml")

    foreach( f ${TEST_FILES} )
        if(NOT IS_DIRECTORY ${f})
            get_filename_component( FILE_NAME ${f} NAME )
            configure_file( ${f} testing/${FILE_NAME} COPYONLY )
            configure_file( ${f} lib/site-packages/roadrunner/testing/${FILE_NAME} COPYONLY )
        endif()
    endforeach()

    foreach( f ${PYTHON_TEST_DATA} )
        if(NOT IS_DIRECTORY ${f})
            get_filename_component( FILE_NAME ${f} NAME )
            configure_file( ${f} lib/site-packages/roadrunner/testing/test_data/${FILE_NAME} COPYONLY )
        endif()
    endforeach()

    foreach( f ${TEST_DATA_XML} )
        if(NOT IS_DIRECTORY ${f})
            get_filename_component( FILE_NAME ${f} NAME )
            configure_file( ${f} lib/site-packages/roadrunner/testing/test_data/${FILE_NAME} COPYONLY )
        endif()
    endforeach()

    add_subdirectory(testing)
    add_subdirectory(autotest)
else()
    message(STATUS "NOT building tests")
endif()


#======================= INSTALL STUFF ========================================================
#What sub folders to move into...?
if(${INSTALL_SBML_MODELS})
    add_subdirectory(models)
endif()

if(INSTALL_CXX_API)
    install(FILES source/rr_pch.h
        DESTINATION include
        COMPONENT rr_core)
endif(INSTALL_CXX_API)


# should we build and install RRPlugins with the roadrunner?
option (INSTALL_RRPLUGINS "Build and install RRPlugins with the roadrunner" ON)

if(INSTALL_RRPLUGINS)
    add_subdirectory(rrplugins)
endif()

install(FILES README.md LICENSE.txt AUTHORS.txt FUNDING.txt
        DESTINATION .
        COMPONENT info
        )

# make or copy installers into root directory
add_subdirectory(installer)

#=== ThirdParties
if(${BORLAND})
    set(CG_RUNTIMES        cc32110MT.dll)
    foreach(runtime ${CG_RUNTIMES})
        install (FILES
            ${THIRD_PARTY_INSTALL_FOLDER}/cg/xe/${runtime}
            DESTINATION bin
            COMPONENT rr_core)
    endforeach()
endif()

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/VERSION.txt.in
        ${CMAKE_CURRENT_BINARY_DIR}/VERSION.txt @ONLY
)

install(
    FILES
    NOTICE.txt
    NEWS.txt
    DESTINATION .
)
# print out configuration summary
ConfigurationSummary()


