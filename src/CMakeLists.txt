CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

configure_file(rrConfigure.h.in rrConfigure.h)
include_directories(${RR_GENERATED_HEADER_PATH}) #we have to do this to get the generated header

OPTION(USE_POCO_NET "Use poco net library" ON)

# pre-process the GitInfo.h.in file and insert the response from git about
# last commit and current branch.
# may not have git, set default replacement strings then.
find_program(GIT git)

if(GIT)
    message("found git: ${GIT}")
    execute_process(
        COMMAND ${GIT} symbolic-ref -q --short HEAD
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        OUTPUT_VARIABLE ROADRUNNER_GIT_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
        )


    execute_process(
        COMMAND ${GIT} log -1 --format=%H
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        OUTPUT_VARIABLE ROADRUNNER_GIT_LAST_COMMIT
        OUTPUT_STRIP_TRAILING_WHITESPACE
        )
else()
    # could not find GIT
    message("could not find git")

    set(ROADRUNNER_GIT_BRANCH "git not found, unknown git branch")
    set(ROADRUNNER_GIT_LAST_COMMIT "git not found, unknown git commit")
endif(GIT)

# writes the results of the git commands to GitInfo.h
message("ROADRUNNER_GIT_BRANCH ${ROADRUNNER_GIT_BRANCH}")
message("ROADRUNNER_GIT_LAST_COMMIT: ${ROADRUNNER_GIT_LAST_COMMIT}")
configure_file(GitInfo.h.in GitInfo.h @ONLY)


set(rrSources
    rrConfig
    rrSteadyStateSolver
    rrConstants
    rrException
    rrGetOptions
    rrLogger
    rrExecutableModel
    SBMLSolver
    SBMLSolverOptions
    rrStringUtils
    rrUtils
    Integrator
    CVODEIntegrator
    Dictionary
    GillespieIntegrator
    RK4Integrator
    rrNLEQInterface
    rrTestSuiteModelSimulation
    rrIniKey
    rrIniSection
    rrIniFile
    rrFileName
    rrRoadRunnerData
    rrSelectionRecord
    ExecutableModelFactory
    rrVersionInfo
    rrSparse
    rrSBMLModelSimulation
    rrSBMLReader
    SBMLValidator
    Variant
    DummyScalarSystem
    conservation/ConservationExtension
    conservation/ConservationDocumentPlugin
    conservation/ConservedMoietyPlugin
    conservation/ConservedMoietyConverter
    testing/CXXExecutableModel
    testing/CXXEnzymeExecutableModel
    testing/CXXBrusselatorExecutableModel
    testing/CXXPiecewiseExecutableModel
    )

# Add the LLVM sources to compilation
if(BUILD_LLVM)
    set(rrSources
        ${rrSources}
        llvm/AssignmentRuleEvaluator
        llvm/ASTNodeCodeGen
        llvm/ASTNodeFactory
        llvm/ModelResources
        llvm/CodeGenBase
        llvm/LLVMCompiler
        llvm/EvalConversionFactorCodeGen
        llvm/EvalInitialConditionsCodeGen
        llvm/EvalRateRuleRatesCodeGen
        llvm/EvalReactionRatesCodeGen
        llvm/EventAssignCodeGen
        llvm/EventTriggerCodeGen
        llvm/EventQueue
        llvm/FunctionResolver
        llvm/LLVMExecutableModel
        llvm/GetValuesCodeGen
        llvm/LoadSymbolResolverBase
        llvm/GetInitialValuesCodeGen
        llvm/GetEventValuesCodeGen
        llvm/KineticLawParameterResolver
        llvm/LLVMModelData
        llvm/ModelDataIRBuilder
        llvm/ModelDataSymbolResolver
        llvm/ModelInitialValueSymbolResolver
        llvm/LLVMModelDataSymbols
        llvm/LLVMModelGenerator
        llvm/ModelGeneratorContext
        llvm/LLVMModelSymbols
        llvm/SetValuesCodeGen
        llvm/SetInitialValuesCodeGen
        llvm/SBMLSupportFunctions
        llvm/EvalVolatileStoichCodeGen
        llvm/SBMLInitialValueSymbolResolver
        llvm/Random
        llvm/DistribFunctionResolver
        )
endif(BUILD_LLVM)

if(ENABLE_FBC)
    set(rrSources
        ${rrSources}
        fbc/LPSolver
        fbc/FBCExecutableModel
        )
endif(ENABLE_FBC)

add_library(sbmlsolver_obj
  OBJECT
  ${rrSources}
  )

target_include_directories(sbmlsolver_obj PUBLIC
  .
  ${SBMLSOLVER_DEP_DIR}/include
  ${SBMLSOLVER_DEP_DIR}/include/rr-libstruct
  ${SBMLSOLVER_DEP_DIR}/include/sbml
  ${SBMLSOLVER_DEP_DIR}/include/cvode
  )


message("SBMLSOLVER_DEP_DIR: ${SBMLSOLVER_DEP_DIR}/include")


target_compile_definitions(sbmlsolver_obj PUBLIC
  EXPORT_RR
  LIBSBML_STATIC
  LIBLAX_STATIC
  STATIC_LIBSTRUCT
  STATIC_PUGI
  STATIC_NLEQ
  POCO_STATIC
  POCO_NO_AUTOMATIC_LIBS
  )

add_library(sbmlsolver_interface INTERFACE)

target_link_libraries(sbmlsolver_interface INTERFACE
  lapack
  sundials_cvode
  sundials_nvecserial
  blas
  nleq-static
  rr-libstruct-static
  PocoNet
  PocoFoundation
  sbml-static
  lapack
  xml2
  f2c
  )

target_include_directories(sbmlsolver_interface INTERFACE
  .
  ${SBMLSOLVER_DEP_DIR}/include
  ${SBMLSOLVER_DEP_DIR}/include/rr-libstruct
  ${SBMLSOLVER_DEP_DIR}/include/sbml
  ${SBMLSOLVER_DEP_DIR}/include/cvode
  ${CMAKE_SOURCE_DIR}/include
  )




if(BUILD_LLVM)
  target_link_libraries(sbmlsolver_obj
    ${LLVM_LIBRARIES}
    )

    target_link_libraries(sbmlsolver_interface INTERFACE
    ${LLVM_LIBRARIES}
    )
endif(BUILD_LLVM)


if(ENABLE_FBC)
  target_link_libraries(sbmlsolver_obj ${COINOR_CLP_LIBRARIES} bz2)
endif(ENABLE_FBC)



# set up linking libs
if(RR_BUILD_SHARED_CORE)

  # shared and static libraries built from the same object files
  add_library(sbmlsolver_shared SHARED $<TARGET_OBJECTS:sbmlsolver_obj>)

  target_link_libraries(sbmlsolver_shared PUBLIC sbmlsolver_interface)

  set_target_properties(sbmlsolver_shared PROPERTIES OUTPUT_NAME "sbmlsolver")


  install(TARGETS sbmlsolver_shared
    RUNTIME DESTINATION bin COMPONENT rr_core
    LIBRARY DESTINATION lib COMPONENT rr_core
    ARCHIVE DESTINATION lib COMPONENT rr_core
    )

endif(RR_BUILD_SHARED_CORE)

# static allways gets build

# shared and static libraries built from the same object files
add_library(sbmlsolver_static STATIC $<TARGET_OBJECTS:sbmlsolver_obj>)
target_link_libraries(sbmlsolver_static PUBLIC sbmlsolver_interface)
set_target_properties(sbmlsolver_static PROPERTIES OUTPUT_NAME "sbmlsolver")


if(BUILD_TEST_TOOLS)
  if(BUILD_LLVM)
    add_subdirectory(llvm_testing)
  endif(BUILD_LLVM)
  
  if(ENABLE_FBC)
    add_subdirectory(fbc_testing)
  endif()
  
  add_subdirectory(testing)
endif()

#--- INSTALL STUFF
if(${INSTALL_CXX_API})

    #if(INSTALL_STATIC_LIB)
    install(TARGETS sbmlsolver_static
        LIBRARY DESTINATION lib COMPONENT rr_core
        ARCHIVE DESTINATION lib COMPONENT rr_core
        )
    #endif()

#     set(install_headers
#         SBMLSolver.h)
    set(subfolders
          .
          tr1proxy
          )

    foreach(subfolder ${subfolders})
       FILE (GLOB headers ${subfolder}/*.h)
       message(STATUS "*** headers *** ${headers}")
       install (   FILES ${headers}
           DESTINATION include/rr/${subfolder}
           COMPONENT rr_core)
    endforeach(subfolder)
endif() #Install CXX API

