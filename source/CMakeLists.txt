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
rrCapability
rrConstants
rrArrayList2
rrArrayListItem
rrException
rrFileLog
rrGetOptions
rrHashTable
rrLogger
rrLogFile
rrLogContainer
rrLoggerUtils
rrLogLevel
rrLogOutput
rrMisc
rrExecutableModel
rrModelState
rrNOMSupport
rrObject
rrPendingAssignment
rrRandom
rrRoadRunner
rrRule
rrSBMLModelSimulation
rrSBMLSymbol
rrSBMLSymbolDependencies
rrSimulationData
rrSimulationSettings
rrStreamWriter
rrStringBuilder
rrStringList
rrStringListContainer
rrStringUtils
rrSymbol
rrSymbolList
rrTextWriter
rrTParameterType
rrTVariableType
rrUtils
rrCGenerator
rrCodeBuilder
rrCompiler
rrCSharpGenerator
rrModelGenerator
rrScanner
rrToken
rrCVODEInterface
rrNLEQInterface
rrDLLEntry
rrCapsSupport
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
rrOSSpecifics
rrModelSharedLibrary
rrBaseParameter
rrRoadRunnerList
rrThreadPool
rrLoadModel
rrRoadRunnerThread
rrLoadModelThread
rrSimulate
rrSimulateThread
rrModelGeneratorFactory
)

#Build both static and shared version
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
endif()

target_link_libraries (${target}    
 sundials_cvode
 sundials_nvecserial
 blas
 lapack
 libf2c${staticLibExt}
 pugi-static
 nleq-static
 rr-libstruct-static
 poco_foundation-static
)
if(UNIX)
target_link_libraries (${target}    
 pthread
 sbml
)
else()
target_link_libraries (${target}    
 libsbml
)
endif()

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
target_link_libraries (${target}-static 
   libsbml-static${staticLibExt}
   sundials_cvode
   sundials_nvecserial
   blas
   lapack
   libf2c${staticLibExt}
   pugi-static
   nleq-static
   rr-libstruct-static
   poco_foundation-static
)

#Libxml we can't build right now
if(${BORLAND})
target_link_libraries (${target}-static libxml2_xe)
endif()

if(${MSVC})
target_link_libraries (${target}-static libxml2)
endif()

if(${UNIX})
target_link_libraries (${target}-static xml2)
endif()

if(BUILD_TESTS)
add_subdirectory(testing)
endif()

#--- INSTALL STUFF
if(${INSTALL_CXX_API})

install(TARGETS ${target} 
        RUNTIME DESTINATION bin COMPONENT rr_core
        LIBRARY DESTINATION lib COMPONENT rr_core
        ARCHIVE DESTINATION lib COMPONENT rr_core
)

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

