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
rrArrayList.cpp
rrArrayListItem.cpp
rrArrayListItemObject.cpp
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
rrCVodeInterface.cpp
rrCvodedll.cpp
rrNLEQInterface.cpp
rrDLLEntry.cpp
rrCapsSupport.cpp
rrStopWatch.cpp
rrEvent.cpp
rrTestSuiteModelSimulation.cpp
mtrand/mtrand.cpp
)

#Build both static and shared version
#Link with static libs
add_library(${target} 	SHARED ${rrSources})

set_property(TARGET ${target}  
                PROPERTY    COMPILE_DEFINITIONS 
                            EXPORT_RR 
                            LIBSBML_STATIC 
                            LIBLAX_STATIC 
                            STATIC_LIBSTRUCT
                            STATIC_PUGI
                            STATIC_NLEQ
                            ) 

#libraries to link with our shared lib
if(${MINGW})
target_link_libraries (${target}    
                                    libsbml
                                    sundials_cvode
                                    sundials_nvecserial
                                    liblapack.dll.a
)
else()
target_link_libraries (${target}    
                                    libsbml-static
                                    sundials_cvode
                                    sundials_nvecserial
                                    blas
                                    lapack
                                    libf2c
                                    pugi-static
                                    nleq-static
                                    rr-libstruct-static
                      )
endif()

#libraries to link with static lib
add_library(${target}-static 	STATIC ${rrSources})


set_property(TARGET ${target}-static 
                PROPERTY    COMPILE_DEFINITIONS 
                            STATIC_RR 
                            LIBSBML_STATIC
                            LIBLAX_STATIC 
                            STATIC_LIBSTRUCT
							STATIC_PUGI
                            STATIC_NLEQ
                            ) 

if(${MINGW})
#can't build static at the moment..
else()
target_link_libraries (${target}-static 
                           libsbml-static
                           sundials_cvode
                           sundials_nvecserial
                           blas
                           lapack
                           #libf2c
						   pugi-static
						   nleq-static
                           rr-libstruct-static
                      )
endif()

#Libxml we can't build right now
if(${BORLAND})
target_link_libraries (${target}        libxml2_xe)
target_link_libraries (${target}-static libxml2_xe)
else()
target_link_libraries (${target}        libxml2)
target_link_libraries (${target}-static libxml2)
endif()


add_subdirectory(testing)
if(${INSTALL_CXX_API})
#--- INSTALL STUFF
if(INSTALL_STATIC_LIB)
    install (   TARGETS ${target}-static	
                DESTINATION lib 
                COMPONENT rr_core)
endif()

#=== Targets -- does not seem to work..
#install(    
#            TARGETS ${target} 
#            RUNTIME DESTINATION bin
#            LIBRARY DESTINATION lib
#            ARCHIVE DESTINATION lib
#            COMPONENT rr_core
#)            

if(${MINGW})
    SET(linkLibrary ${lib_path}/lib${target}.dll.a)
    SET(sharedLibrary ${exe_path}/lib${target}.dll)
else()
    SET(linkLibrary ${lib_path}/${target}.lib)
    SET(sharedLibrary ${exe_path}/${target}.dll)
endif()

#=== Shared target
    install (   FILES ${sharedLibrary}	
                DESTINATION bin 
                COMPONENT rr_core)

    install (   FILES ${linkLibrary}	
                DESTINATION lib 
                COMPONENT rr_core)


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

