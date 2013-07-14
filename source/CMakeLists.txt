CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

set(target roadrunner)

configure_file(rrConfigure.h.in rrConfigure.h)
include_directories(${RR_GENERATED_HEADER_PATH}) #we have to do this to get the generated header

include_directories(
    .
    ${THIRD_PARTY_INSTALL_FOLDER}/include
    ${THIRD_PARTY_INSTALL_FOLDER}/include/rr-libstruct
    ${THIRD_PARTY_INSTALL_FOLDER}/include/sbml
    ${THIRD_PARTY_INSTALL_FOLDER}/include/cvode
    )

set(rrSources
    rrParameter
    rrParameters
    rrSteadyStateSolver
    rrCapability
    rrCapabilities
    rrConstants
    rrException
    rrFileLog
    rrGetOptions
    rrHashTable
    rrLogFile
    rrLogContainer
    rrLoggerUtils
    rrLogLevel
    rrLogOutput
    rrCompiledExecutableModel
    rrExecutableModel
    rrCompiledModelState
    rrNOMSupport
    rrObject
    rrPendingAssignment
    rrRandom
    rrRoadRunner
    rrRule
    rrSBMLModelSimulation
    rrSBMLSymbol
    rrSBMLSymbolDependencies
    rrSimulationSettings
    rrStreamWriter
    rrStringBuilder
    rrStringList
    rrStringListContainer
    rrStringUtils
    rrSymbol
    rrSymbolList
    rrUtils
    rrModelSymbols
    rrModelGenerator
    rrCompiledModelGenerator
    rrCModelGenerator
    rrCSharpModelGenerator
    rrCodeBuilder
    rrCompiler
    rrCPlugin
    rrScanner
    rrToken
    rrCVODEInterface
    rrNLEQInterface
    rrStopWatch
    rrEvent
    rrTestSuiteModelSimulation
    rrNewArrayList
    rrNewArrayListItem
    rrNewArrayListItemObject
    rrIniKey
    rrIniSection
    rrIniFile
    rrFileName
    mtrand/mtrand
    rrPluginManager
    rrPlugin
    rrModelSharedLibrary
    rrBaseParameter
    rrRoadRunnerList
    rrThreadPool
    rrLoadModel
    rrLoadModelFromFile
    rrRoadRunnerThread
    rrLoadModelThread
    rrSimulate
    rrSimulateThread
    rrRoadRunnerData
    rrSelectionRecord
    rrMinimizationData
    rrModelGeneratorFactory
    rrNoise
    rrVersionInfo
    rrSparse
    )

# Add the LLVM sources to compilation
if(BUILD_LLVM)
    set(rrSources
        ${rrSources}
        rrLLVMAssignmentRuleEvaluator
        rrLLVMASTNodeCodeGen
        rrLLVMASTNodeFactory
        rrLLVMCodeGenerator
        rrLLVMCompiler
        rrLLVMExecutableModel
        rrLLVMInitialValueCodeGen
        rrLLVMModelDataIRBuilder
        rrLLVMModelDataSymbols
        rrLLVMModelGenerator
        rrLLVMModelGeneratorContext
        )
endif(BUILD_LLVM)

# set up linking libs
if(RR_BUILD_SHARED_CORE)
    #Link with static libs
    add_library(${target} SHARED ${rrSources})

    set_property(TARGET ${target}
        PROPERTY  COMPILE_DEFINITIONS
        EXPORT_RR
        LIBSBML_STATIC
        LIBLAX_STATIC
        STATIC_LIBSTRUCT
        STATIC_PUGI
        STATIC_NLEQ
        POCO_STATIC
        POCO_NO_AUTOMATIC_LIBS
        )

    #libraries to link with our shared lib
    if(UNIX)
        set(staticLibExt ".a")
    else()
        set(staticLibExt "")
    endif(UNIX)

    target_link_libraries (${target}
        sundials_cvode
        sundials_nvecserial
        blas
        lapack
        pugi-static
        nleq-static
        rr-libstruct-static
        poco_foundation-static
        )

    if(UNIX)
        target_link_libraries (${target}
            pthread
            sbml
            dl
            f2c${staticLibExt}
            )
    else()
        target_link_libraries (${target}
            libf2c
            libsbml
            )
    endif(UNIX)


    if(${INSTALL_CXX_API})
        install(TARGETS ${target}
            RUNTIME DESTINATION bin COMPONENT rr_core
            LIBRARY DESTINATION lib COMPONENT rr_core
            ARCHIVE DESTINATION lib COMPONENT rr_core
            )
    endif()


    if(BUILD_LLVM)
        target_link_libraries(${target}
            ${LLVM_LIBRARIES}
            )
    endif(BUILD_LLVM)


endif(RR_BUILD_SHARED_CORE)

# static allways gets build

add_library(${target}-static STATIC ${rrSources})

set_property(TARGET ${target}-static
    PROPERTY COMPILE_DEFINITIONS
    STATIC_RR
    LIBSBML_USE_CPP_NAMESPACE
    LIBSBML_STATIC
    LIBLAX_STATIC
    STATIC_LIBSTRUCT
    STATIC_PUGI
    STATIC_NLEQ
    POCO_STATIC
    POCO_NO_AUTOMATIC_LIBS
    )

#libraries to link with static lib
#======== We need to split up this one to satisfy linking in both windows and posix.
#=== windows need libsbml-static and libf2c and posix uses sbml-static and f2c
target_link_libraries (${target}-static
    sundials_cvode
    sundials_nvecserial
    blas
    lapack
    pugi-static
    nleq-static
    rr-libstruct-static
    poco_foundation-static
    )

if(WIN32)
    target_link_libraries (${target}-static
        libsbml-static
        libf2c
        )
endif(WIN32)


if(UNIX)
    target_link_libraries (${target}-static
        sbml-static
        f2c
        xml2
        dl
        pthread
        )
endif(UNIX)

if(BUILD_LLVM)
    target_link_libraries(${target}-static
        ${LLVM_LIBRARIES}
        )
endif(BUILD_LLVM)

#Libxml we can't build right now
if(${BORLAND})
    target_link_libraries (${target}-static
        libxml2_xe
        )
endif()

if(${MSVC})
    target_link_libraries(${target}-static libxml2)
endif()

if(BUILD_LLVM)
    add_subdirectory(llvm_testing)
endif(BUILD_LLVM)

if(BUILD_TESTS)
    add_subdirectory(testing)
endif()

#--- INSTALL STUFF
if(${INSTALL_CXX_API})

    #if(INSTALL_STATIC_LIB)
    install(TARGETS ${target}-static
        LIBRARY DESTINATION lib COMPONENT rr_core
        ARCHIVE DESTINATION lib COMPONENT rr_core
        )
    #endif()

    #=== HEADERS
    set(subfolders
        .
        ./mtrand
        )

    foreach(subfolder ${subfolders})
        FILE (GLOB headers ${subfolder}/*.h)
        install (   FILES ${headers}
            DESTINATION include/rr/${subfolder}
            COMPONENT rr_core)
    endforeach(subfolder)
endif() #Install CXX API

