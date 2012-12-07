CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

set(target roadrunner) 

include_directories(
. 
${THIRD_PARTY_INSTALL_FOLDER}/include
${THIRD_PARTY_INSTALL_FOLDER}/include/rr-libstruct
${THIRD_PARTY_INSTALL_FOLDER}/include/sbml
${THIRD_PARTY_INSTALL_FOLDER}/include/cvode
)

set(rrSources 
rrCapability.cpp
rrCapabilitiesSection.cpp
rrConstants.cpp
rrArrayList2
rrArrayListItem
rrException.cpp
rrFileLog.cpp
rrGetOptions.cpp
rrHashTable.cpp
rrIModel.cpp
rrLogger.cpp
rrLogFile.cpp
rrLogContainer.cpp
rrLoggerUtils.cpp
rrLogLevel.cpp
rrLogOutput.cpp
rrMisc.cpp
rrModelFromC.cpp
rrModelState.cpp
rrNOMSupport.cpp
rrObject.cpp
rrPendingAssignment.cpp
rrRandom.cpp
rrRoadRunner.cpp
rrRule.cpp
rrSBMLModelSimulation.cpp
rrSBMLSymbol.cpp
rrSBMLSymbolDependencies.cpp
rrSimulationData.cpp
rrSimulationSettings.cpp
rrStreamWriter.cpp
rrStringBuilder.cpp
rrStringList.cpp
rrStringListContainer.cpp
rrStringUtils.cpp
rrSymbol.cpp
rrSymbolList.cpp
rrTextWriter.cpp
rrTParameterType.cpp
rrTVariableType.cpp
rrUtils.cpp
rrCGenerator.cpp
rrCodeBuilder.cpp
rrCompiler.cpp
rrCSharpGenerator.cpp
rrModelGenerator.cpp
rrScanner.cpp
rrToken.cpp
rrCVODEInterface.cpp
rrCVODE_DLL.cpp
rrNLEQInterface.cpp
rrDLLEntry.cpp
rrCapsSupport.cpp
rrStopWatch.cpp
rrEvent.cpp
rrTestSuiteModelSimulation.cpp
rrNewArrayList
rrNewArrayListItem
rrNewArrayListItemObject
mtrand/mtrand.cpp
)

#Build both static and shared version
#Link with static libs
# add_library(${target} SHARED ${rrSources})
# 
# set_property(TARGET ${target}  
#  PROPERTY  COMPILE_DEFINITIONS 
#  EXPORT_RR 
#  LIBSBML_STATIC 
#  LIBLAX_STATIC 
#  STATIC_LIBSTRUCT
#  STATIC_PUGI
#  STATIC_NLEQ
#  POCO_STATIC
#  POCO_NO_AUTOMATIC_LIBS
# ) 
# 
#libraries to link with our shared lib
if(UNIX)
set(staticLibExt ".a")
else()
set(staticLibExt "")
endif()
# 
# target_link_libraries (${target}    
#  libsbml-static${staticLibExt}
#  sundials_cvode
#  sundials_nvecserial
#  blas
#  lapack
#  libf2c${staticLibExt}
#  pugi-static
#  nleq-static
#  rr-libstruct-static
#  poco_foundation-static
# )

#libraries to link with static lib
add_library(${target}-static STATIC ${rrSources})

set_property(TARGET ${target}-static 
PROPERTY COMPILE_DEFINITIONS 
STATIC_RR 
LIBSBML_STATIC
LIBLAX_STATIC 
STATIC_LIBSTRUCT
STATIC_PUGI
STATIC_NLEQ
POCO_STATIC
POCO_NO_AUTOMATIC_LIBS
) 

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
# target_link_libraries (${target}        libxml2_xe)
target_link_libraries (${target}-static libxml2_xe)
else()
# target_link_libraries (${target}        libxml2${staticLibExt})
target_link_libraries (${target}-static libxml2${staticLibExt})
endif()


if(BUILD_TESTS)
add_subdirectory(testing)
endif()

#--- INSTALL STUFF
if(${INSTALL_CXX_API})

# install(TARGETS ${target} 
#         LIBRARY DESTINATION lib 
#         RUNTIME DESTINATION bin 
#         ARCHIVE DESTINATION lib
# )

if(INSTALL_STATIC_LIB)
install(TARGETS ${target}-static 
        LIBRARY DESTINATION lib 
        ARCHIVE DESTINATION lib)
endif()

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

