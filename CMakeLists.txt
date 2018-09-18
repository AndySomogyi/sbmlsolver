cmake_minimum_required(VERSION 2.8)
project(SBMLSolver)

# These will be the defaults ...
if(WIN32)
    SET(RR_BUILDS_ROOT      "c:/builds")
    SET(RR_INSTALLS_ROOT    "c:/installs")


    if(MSVC)
        set(FOLDER_POSTFIX "vs")
    endif()

    if(UNIX)
        set(FOLDER_POSTFIX "gcc")
    endif()

    set(SBMLSOLVER_DEP_DIR ${RR_INSTALLS_ROOT}/${FOLDER_POSTFIX}/release CACHE PATH "Directory containing ThirdParty libraries.")
    if("${SBMLSOLVER_DEP_DIR}" STREQUAL "")
        set(SBMLSOLVER_DEP_DIR ${RR_INSTALLS_ROOT}/${FOLDER_POSTFIX}/release CACHE PATH "Directory containing ThirdParty libraries" FORCE)
    endif()
else()
    # Default the third party dir to a more conventional unix type path
    # This would typically be /usr/local, $HOME/local or something like that.
    set(SBMLSOLVER_DEP_DIR ${CMAKE_INSTALL_PREFIX} CACHE PATH "Directory containing ThirdParty libraries.")
endif(WIN32)

# add cmake files to find things, prepend with our path
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/ ${CMAKE_MODULE_PATH})


set(LLVM_INSTALL_PREFIX CACHE PATH "If LLVM was built using CMake, this is the location where it was installed")

# determine if LLVM was installed using CMake
if(EXISTS ${LLVM_INSTALL_PREFIX}/share/llvm/cmake)
    message(STATUS "Found CMake built LLVM")

    option (BUILD_LLVM "Build the LLVM back end" ON)
    set(BUILD_LLVM ON)
    set(LLVM_FOUND TRUE)

    # Use the propper LLVM supplied CMake files.
    # prepend the cmake module path with the LLVM modules
    set(CMAKE_MODULE_PATH "${LLVM_INSTALL_PREFIX}/share/llvm/cmake" ${CMAKE_MODULE_PATH})

    include(LLVMConfig)

    # we're building a JIT compiler with support for binary code (no interpreter):
    # this sets the LLVM_LIBRARIES var to be the list of required LLVM libs
    # to link with.
    llvm_map_components_to_libraries(LLVM_LIBRARIES jit native)

else()
    message(STATUS "Looking for LLVM installed without CMake")

    set(LLVM_MIN_VERSION       "3000000")
    set(LLVM_MIN_VERSION_TEXT  "3.0")

    # look for LLVM using the llvm-config program
    find_package(LLVM)
endif()


# should we build and install Testing tools?
option (BUILD_TEST_TOOLS "Build and install test tools" ON)

# should we use LLVM ?
option (BUILD_LLVM "Build the LLVM back end" ON)

# should we use FBC ?
option (ENABLE_FBC "Enable Flux Balance Constraint Modeling" OFF)

if(ENABLE_FBC)
  # specify rood dir of constraint solver"
  set(COINOR_PREFIX "" CACHE PATH "The install prefix of the Coin-Or linear programming library (CLP). Required for FBC, leave blank for system default path.")
  find_package(COINOR_CLP REQUIRED)

  include_directories(${COINOR_CLP_INCLUDE_DIR})
  
  add_definitions(-DENABLE_FBC)
endif()
                

# should we build the swig python wrapper?
option (BUILD_PYTHON "build the SWIG generated python wrapper" OFF)

if(BUILD_LLVM AND NOT LLVM_FOUND)
    message(FATAL_ERROR "BUILD_LLVM is enabled, but no LLVM installation was found")
endif()

mark_as_advanced(
    INSTALL_APPS
    INSTALL_CXX_API
    INSTALL_C_API
    INSTALL_C_API_PYTHON
    INSTALL_STATIC_LIB
    )

set(RR_GENERATED_HEADER_PATH ${CMAKE_CURRENT_BINARY_DIR}/src)
set(RR_ROADRUNNER_INSTALL_PATH ${CMAKE_INSTALL_PREFIX})
set(RR_ROADRUNNER_INSTALL_LIB_PATH ${RR_ROADRUNNER_INSTALL_PATH}/lib)
set(RR_SOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

# new clang on OSX and msvc use std namespace. 
option (USE_TR1_CXX_NS          "Use the tr1 c++ namespace for swig smart pointer" ${CMAKE_COMPILER_IS_GNUCXX})
OPTION (INSTALL_CXX_API         "Generate C++ api"                               ON )
OPTION (INSTALL_C_API           "Generate C API"                                 ON )
OPTION (INSTALL_C_API_PYTHON    "Package ctypes  Python wrapper for the C API"   ON )
OPTION (INSTALL_APPS            "Build and install Apps"                         ON )
OPTION (INSTALL_EXAMPLES        "Build and install Examples"                     OFF)
OPTION (INSTALL_STATIC_LIB      "Install RoadRunner CXX static lib"              OFF)
OPTION (RR_BUILD_SHARED_CORE    "Build RoadRunner Core Shared library"           ON )
OPTION (BUILD_TESTS             "Build the SBML C API test suite"                ON )
OPTION (INSTALL_SBML_MODELS     "Install SBML Models"                            ON )

#Setup so that sbmlsolver is always built as a dll and linked statically with 'as much as possible'
set(BUILD_SHARED_LIBS            ON)

set(RR_ROOT                      ${CMAKE_CURRENT_SOURCE_DIR})
set(RR_SOURCE_ROOT               ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(THIRD_PARTY_FOLDER           ${CMAKE_CURRENT_SOURCE_DIR}/third_party)

set(exe_path                     ${PROJECT_BINARY_DIR}/bin)
set(lib_path                     ${PROJECT_BINARY_DIR}/lib)

message(STATUS "Using third party library prefix of ${SBMLSOLVER_DEP_DIR}")
message(STATUS "Installing RoadRunner to ${CMAKE_INSTALL_PREFIX}")

set(CMAKE_BUILD_TYPE "Debug" CACHE STRING
    "Choose the build type. The options are: None (CMAKE_CXX_FLAGS or
     CMAKE_C_FLAGS are used), Debug, Release, RelWithDebInfo, MinSizeRel.")
if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING
        "Choose the build type. The options are: None (CMAKE_CXX_FLAGS or
  CMAKE_C_FLAGS are used), Debug, Release, RelWithDebInfo, MinSizeRel." FORCE)
endif()

IF(WIN32)
    IF(${MSVC})
        IF(CMAKE_BUILD_TYPE MATCHES "Debug")
            set(exe_path ${exe_path}/Debug)
            set(lib_path ${lib_path}/Debug)
        ELSE(CMAKE_BUILD_TYPE MATCHES "Debug")
               set(exe_path ${exe_path}/Release)
               set(lib_path ${lib_path}/Release)
        ENDIF(CMAKE_BUILD_TYPE MATCHES "Debug")
    ELSE(WIN32)
   # Linux
    ENDIF(${MSVC})
ENDIF(WIN32)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY     ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY     ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY     ${PROJECT_BINARY_DIR}/bin)

include_directories(
    ${RR_ROOT}
    ${THIRD_PARTY_FOLDER}
)

# TODO libxml2 handling is excedingly hackish, it is all homegrown
# manually linking crap, need to fix this!

if (WIN32)
    message(STATUS "adding ${SBMLSOLVER_DEP_DIR}/include/libxml2 to include path")
    include_directories(${SBMLSOLVER_DEP_DIR}/include/libxml2)
else()
    #UNIX
    find_package(LibXml2 REQUIRED)
	message(STATUS "found libxml2, include dir: ${LIBXML2_INCLUDE_DIR}") 
	include_directories(${LIBXML2_INCLUDE_DIR})
endif(WIN32)


if (${BUILD_LLVM})
    link_directories(
        ${LIBRARY_OUTPUT_PATH}
        ${THIRD_PARTY_FOLDER}/dependencies/libsbml/lib
        ${SBMLSOLVER_DEP_DIR}/lib
        ${LLVM_LIBRARY_DIRS}
    )
else()
    link_directories(
        ${LIBRARY_OUTPUT_PATH}
        ${THIRD_PARTY_FOLDER}/dependencies/libsbml/lib
        ${SBMLSOLVER_DEP_DIR}/lib
    )
endif (${BUILD_LLVM})

if(${MSVC})
    add_definitions(
        -DPOCO_NO_AUTOMATIC_LIBS
        -DCRT_NONSTDC_NO_DEPRECATE
        -D_CRT_SECURE_NO_WARNINGS    #MS wanting you to changes basically all standard C functions :(
        )
        
    #  4251 About exporting std classes
    #  4018 Comparing unsigned/signed ints
    #  4996 Deprecated functions   
    add_definitions( "/wd4251 /wd4018 /wd4996 /nologo" )   

    SET (CMAKE_CXX_FLAGS             "${CMAKE_CXX_FLAGS} /EHsc ")
    SET (CMAKE_SHARED_LINKER_FLAGS     "${CMAKE_SHARED_LINKER_FLAGS} ") #/FORCE:MULTIPLE")

endif()

#=== COMPILER FLAGS
#Common compiler definitions
add_definitions(
    -DLIBSBML_USE_CPP_NAMESPACE
)

if(${MINGW})
    SET (CMAKE_CXX_FLAGS             "${CMAKE_CXX_FLAGS} -std=gnu++0x")
endif()


if(UNIX)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
endif()

if(BUILD_LLVM)
    message(STATUS "using LLVM, version ${LLVM_VERSION_MAJOR}.${LLVM_VERSION_MINOR}.${LLVM_VERSION_PATCH}")

    message(STATUS "LLVM_LIBRARIES: ${LLVM_LIBRARIES}")
    message(STATUS "LLVM_DEFINITIONS: ${LLVM_DEFINITIONS}")
    message(STATUS "LLVM_INCLUDE_DIRS: ${LLVM_INCLUDE_DIRS}")
    message(STATUS "LLVM_LIBRARY_DIRS: ${LLVM_LIBRARY_DIRS}")
    add_definitions(-DBUILD_LLVM)
    add_definitions(${LLVM_DEFINITIONS})
    include_directories(${LLVM_INCLUDE_DIRS})
else()
    message(STATUS "NOT using LLVM")
endif(BUILD_LLVM)


# public include files
add_subdirectory(include)

# We alway have to build the sbmlsolver core, everything depends on this
add_subdirectory(src)


if(INSTALL_CXX_API)
    if(INSTALL_APPS)
        add_subdirectory(apps)
    endif()

    if(INSTALL_EXAMPLES)
        add_subdirectory(examples)
    endif()
endif(INSTALL_CXX_API)

add_subdirectory(wrappers)

if(BUILD_TEST_TOOLS)
    message(STATUS "building tests")
    add_subdirectory(testing)
else()
    message(STATUS "NOT building tests")
endif(BUILD_TEST_TOOLS)


#======================= INSTALL STUFF ========================================================
#What sub folders to move into...?
if(${INSTALL_SBML_MODELS})
    add_subdirectory(models)
endif()

if(INSTALL_CXX_API)
    install(FILES src/rr_pch.h
        DESTINATION include
        COMPONENT rr_core)
endif(INSTALL_CXX_API)

#install(FILES README.md LICENSE.md AUTHORS.md FUNDING.md
#        DESTINATION .
#        COMPONENT info
#        )

# make or copy installers into root directory
add_subdirectory(installer)


if(WIN32)
#One day these may be statically linked
set(RUNTIMES libxml2.DLL iconv.dll zlib1.dll)
foreach(runtime ${RUNTIMES})
    install(    FILES     ${THIRD_PARTY_FOLDER}/dependencies/libsbml/bin/${runtime}
        DESTINATION bin
        COMPONENT rr_core)
endforeach()
endif()

#install(
#    FILES
#    NOTICE.txt
#    VERSION.txt
#    NEWS.txt
#    DESTINATION .
#)


if(WIN32)
    install(
        FILES win32_bin_README.txt DESTINATION bin/ RENAME README.txt
    )
endif(WIN32)
