#####################################
#
set(target roadrunner)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/rrConfigure.h.in ${CMAKE_CURRENT_SOURCE_DIR}/rrConfigure.h)

#####################################
# Get a list of source file to build
#

# add core sources. These are always needed
set(rrCoreSources
        rrConfig.cpp
        rrSteadyStateSolver.cpp
        rrConstants.cpp
        rrException.cpp
        rrGetOptions.cpp
        rrLogger.cpp
        rrExecutableModel.cpp
        rrRoadRunner.cpp
        rrRoadRunnerOptions.cpp
        rrStringUtils.cpp
        rrUtils.cpp
        Integrator.cpp
        IntegratorRegistration.cpp
        CVODEIntegrator.cpp
        Dictionary.cpp
        GillespieIntegrator.cpp
        RK4Integrator.cpp
        RK45Integrator.cpp
        NLEQ1Solver.cpp
        NLEQ2Solver.cpp
        rrNLEQ1Interface.cpp
        rrNLEQ2Interface.cpp
        rrTestSuiteModelSimulation.cpp
        rrIniKey.cpp
        rrIniSection.cpp
        rrIniFile.cpp
        rrFileName.cpp
        rrRoadRunnerData.cpp
        rrSelectionRecord.cpp
        ExecutableModelFactory.cpp
        rrVersionInfo.cpp
        rrSparse.cpp
        rrSBMLModelSimulation.cpp
        rrSBMLReader.cpp
        SBMLValidator.cpp
        SteadyStateSolver.cpp
        Solver.cpp
        SolverRegistration.cpp
        Variant.cpp
        conservation/ConservationExtension.cpp
        conservation/ConservationDocumentPlugin.cpp
        conservation/ConservedMoietyPlugin.cpp
        conservation/ConservedMoietyConverter.cpp
        testing/CXXExecutableModel.cpp
        testing/CXXEnzymeExecutableModel.cpp
        testing/CXXBrusselatorExecutableModel.cpp
        testing/CXXPiecewiseExecutableModel.cpp
        rrConfig.h
        rrSteadyStateSolver.h
        rrConstants.h
        rrException.h
        rrGetOptions.h
        rrLogger.h
        rrExecutableModel.h
        rrRoadRunner.h
        rrRoadRunnerOptions.h
        rrStringUtils.h
        rrUtils.h
        Integrator.h
        IntegratorRegistration.h
        CVODEIntegrator.h
        Dictionary.h
        GillespieIntegrator.h
        RK4Integrator.h
        RK45Integrator.h
        NLEQ1Solver.h
        NLEQ2Solver.h
        rrNLEQ1Interface.h
        rrNLEQ2Interface.h
        rrTestSuiteModelSimulation.h
        rrIniKey.h
        rrIniSection.h
        rrIniFile.h
        rrFileName.h
        rrRoadRunnerData.h
        rrSelectionRecord.h
        ExecutableModelFactory.h
        rrVersionInfo.h
        rrSparse.h
        rrSBMLModelSimulation.h
        rrSBMLReader.h
        SBMLValidator.h
        SteadyStateSolver.h
        Solver.h
        SolverRegistration.h
        Variant.h
        conservation/ConservationExtension.h
        conservation/ConservationDocumentPlugin.h
        conservation/ConservedMoietyPlugin.h
        conservation/ConservedMoietyConverter.h
        testing/CXXExecutableModel.h
        testing/CXXEnzymeExecutableModel.h
        testing/CXXBrusselatorExecutableModel.h
        testing/CXXPiecewiseExecutableModel.h
        )

set(rrSources ${rrCoreSources})

# https://cmake.org/cmake/help/v3.3/prop_sf/GENERATED.html
set_source_files_properties(rrVersionInfo.h PROPERTIES GENERATED TRUE)

# Add the LLVM sources to compilation
if (BUILD_LLVM)
    set(rrLLVMSources
            llvm/AssignmentRuleEvaluator.cpp
            llvm/ASTNodeCodeGen.cpp
            llvm/ASTNodeFactory.cpp
            llvm/ModelResources.cpp
            llvm/CodeGenBase.cpp
            llvm/LLVMCompiler.cpp
            llvm/EvalConversionFactorCodeGen.cpp
            llvm/EvalInitialConditionsCodeGen.cpp
            llvm/EvalRateRuleRatesCodeGen.cpp
            llvm/EvalReactionRatesCodeGen.cpp
            llvm/EventAssignCodeGen.cpp
            llvm/EventTriggerCodeGen.cpp
            llvm/EventQueue.cpp
            llvm/FunctionResolver.cpp
            llvm/LLVMExecutableModel.cpp
            llvm/GetValuesCodeGen.cpp
            llvm/LoadSymbolResolverBase.cpp
            llvm/GetInitialValuesCodeGen.cpp
            llvm/GetEventValuesCodeGen.cpp
            llvm/KineticLawParameterResolver.cpp
            llvm/LLVMModelData.cpp
            llvm/ModelDataIRBuilder.cpp
            llvm/ModelDataSymbolResolver.cpp
            llvm/ModelInitialValueSymbolResolver.cpp
            llvm/LLVMModelDataSymbols.cpp
            llvm/LLVMModelGenerator.cpp
            llvm/ModelGeneratorContext.cpp
            llvm/LLVMModelSymbols.cpp
            llvm/SetValuesCodeGen.cpp
            llvm/SetInitialValuesCodeGen.cpp
            llvm/SBMLSupportFunctions.cpp
            llvm/EvalVolatileStoichCodeGen.cpp
            llvm/SBMLInitialValueSymbolResolver.cpp
            llvm/Random.cpp
            llvm/AssignmentRuleEvaluator.h
            llvm/ASTNodeCodeGen.h
            llvm/ASTNodeFactory.h
            llvm/ModelResources.h
            llvm/CodeGenBase.h
            llvm/LLVMCompiler.h
            llvm/EvalConversionFactorCodeGen.h
            llvm/EvalInitialConditionsCodeGen.h
            llvm/EvalRateRuleRatesCodeGen.h
            llvm/EvalReactionRatesCodeGen.h
            llvm/EventAssignCodeGen.h
            llvm/EventTriggerCodeGen.h
            llvm/EventQueue.h
            llvm/FunctionResolver.h
            llvm/LLVMExecutableModel.h
            llvm/GetValuesCodeGen.h
            llvm/LoadSymbolResolverBase.h
            llvm/GetInitialValuesCodeGen.h
            llvm/GetEventValuesCodeGen.h
            llvm/KineticLawParameterResolver.h
            llvm/LLVMModelData.h
            llvm/ModelDataIRBuilder.h
            llvm/ModelDataSymbolResolver.h
            llvm/ModelInitialValueSymbolResolver.h
            llvm/LLVMModelDataSymbols.h
            llvm/LLVMModelGenerator.h
            llvm/ModelGeneratorContext.h
            llvm/LLVMModelSymbols.h
            llvm/SetValuesCodeGen.h
            llvm/SetInitialValuesCodeGen.h
            llvm/SBMLSupportFunctions.h
            llvm/EvalVolatileStoichCodeGen.h
            llvm/SBMLInitialValueSymbolResolver.h
            llvm/Random.h
            )

    # set up appropriate flags for using LLVM
    if (LLVM_FLAGS)
        set_source_files_properties(${rrLLVMSources} PROPERTIES COMPILE_FLAGS ${LLVM_FLAGS})
    endif ()
    # add LLVM-based model sources to list
    set(rrSources ${rrSources} ${rrLLVMSources})
endif (BUILD_LLVM)

# Add C legacy code to sources if needed
if (BUILD_LEGACY_C)
    set(rrSources
            ${rrSources}
            c/rrStringList.cpp
            c/mtrand.cpp
            c/rrStringBuilder.cpp
            c/rrStringListContainer.cpp
            c/rrSBMLSymbolDependencies.cpp
            c/rrSBMLSymbol.cpp
            c/rrHashTable.cpp
            c/rrRule.cpp
            c/rrModelSymbols.cpp
            c/rrSymbol.cpp
            c/rrSymbolList.cpp
            c/rrCompiledModelState.cpp
            c/rrNOMSupport.cpp
            c/rrPendingAssignment.cpp
            c/rrCompiledModelGenerator.cpp
            c/rrCModelGenerator.cpp
            c/rrCodeBuilder.cpp
            c/rrCCompiler.cpp
            c/rrScanner.cpp
            c/rrToken.cpp
            c/rrEvent.cpp
            c/rrModelSharedLibrary.cpp
            c/rrCModelDataUtil.cpp
            c/rrCompiledExecutableModel.cpp
            c/rrRandom.cpp
            c/rrStringList.h
            c/mtrand.h
            c/rrStringBuilder.h
            c/rrStringListContainer.h
            c/rrSBMLSymbolDependencies.h
            c/rrSBMLSymbol.h
            c/rrHashTable.h
            c/rrRule.h
            c/rrModelSymbols.h
            c/rrSymbol.h
            c/rrSymbolList.h
            c/rrCompiledModelState.h
            c/rrNOMSupport.h
            c/rrPendingAssignment.h
            c/rrCompiledModelGenerator.h
            c/rrCModelGenerator.h
            c/rrCodeBuilder.h
            c/rrCCompiler.h
            c/rrScanner.h
            c/rrToken.h
            c/rrEvent.h
            c/rrModelSharedLibrary.h
            c/rrCModelDataUtil.h
            c/rrCompiledExecutableModel.h
            c/rrRandom.h
            )
endif (BUILD_LEGACY_C)


configure_file(rrConfigure.h.in rrConfigure.h)


set(LINK_LIBRARIES
        "sundials_cvode"
        "sundials_nvecserial"
        "blas"
        "lapack"
        "nleq1-static"
        "nleq2-static"
        "rr-libstruct-static"
        "PocoNet"
        "PocoFoundation"
        )

if (UNIX)
    set(LINK_LIBRARIES "${LINK_LIBRARIES}" "xml2")
elseif (${MSVC})
    set(LINK_LIBRARIES "${LINK_LIBRARIES} libxml2")
elseif (${BORLAND})
    set(LINK_LIBRARIES "${LINK_LIBRARIES} libxml2_xe")
endif ()


#Link with static libs
add_library(${target}-static STATIC ${rrSources})

# set version
set_target_properties(${target}-static PROPERTIES SOVERSION ${ROADRUNNER_VERSION})


set_property(TARGET ${target}-static
        PROPERTY COMPILE_DEFINITIONS
        STATIC_RR
        LIBSBML_USE_CPP_NAMESPACE
        LIBSBML_STATIC
        LIBLAX_STATIC
        STATIC_LIBSTRUCT
        STATIC_PUGI
        STATIC_NLEQ1
        STATIC_NLEQ2
        POCO_STATIC
        POCO_NO_AUTOMATIC_LIBS
        )

target_link_libraries (${target}-static
    sundials_cvode
    sundials_nvecserial
    blas
    lapack
    nleq1-static
    nleq2-static
    rr-libstruct-static
    PocoNet
    PocoFoundation
    )

target_include_directories(${target}-static PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        /mnt/d/rrbuild/roadrunner/third_party/libSBML-5.18.1-experimental-Source/install-GNU/include
        /mnt/d/rrbuild/roadrunner/third_party/poco_1.5.3/install-GNU/include
        /mnt/d/rrbuild/roadrunner/third_party/rr-libstruct
        )

if (BUILD_SHARED_LIBS)

endif (BUILD_SHARED_LIBS)





















