#####################################
#
set(target roadrunner)

# pre-process the GitInfo.h.in file and insert the response from git about
# last commit and current branch.
# may not have git, set default replacement strings then.
find_program(GIT git)

if (GIT)
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
else ()
    # could not find GIT
    message("could not find git")

    set(ROADRUNNER_GIT_BRANCH "git not found, unknown git branch")
    set(ROADRUNNER_GIT_LAST_COMMIT "git not found, unknown git commit")
endif (GIT)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/rrVersionInfo.h DESTINATION include/rr)

# writes the results of the git commands to GitInfo.h
message("ROADRUNNER_GIT_BRANCH ${ROADRUNNER_GIT_BRANCH}")
message("ROADRUNNER_GIT_LAST_COMMIT: ${ROADRUNNER_GIT_LAST_COMMIT}")


# Configures
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/rrConfigure.h.in ${CMAKE_CURRENT_SOURCE_DIR}/rrConfigure.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/rrVersionInfo.h.in ${CMAKE_CURRENT_SOURCE_DIR}/rrVersionInfo.h @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/GitInfo.h.in ${CMAKE_CURRENT_SOURCE_DIR}/GitInfo.h @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/roadrunner.pc.in ${CMAKE_CURRENT_SOURCE_DIR}/roadrunner.pc @ONLY)

# Create the .pc file
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/roadrunner.pc DESTINATION lib/pkgconfig)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/roadrunner.pc DESTINATION share/pkgconfig)


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
        testing/CXXExecutableModel.cpp
        testing/CXXEnzymeExecutableModel.cpp
        testing/CXXBrusselatorExecutableModel.cpp
        testing/CXXPiecewiseExecutableModel.cpp
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

    if (NOT LLVM_FLAGS)
        message(FATAL_ERROR "No LLVM_FLAGS variable found")
    endif ()
    # Don't use add_definitions for LLVM flags per http://stackoverflow.com/questions/13638408/cmake-override-compile-flags-for-single-files
    # Line below kept commented out on purpose - we don't want this.
    #     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LLVM_FLAGS}") # Set per-source
    # set up appropriate flags for using LLVM
    set_source_files_properties(${rrLLVMSources} PROPERTIES COMPILE_FLAGS ${LLVM_FLAGS})
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

##############################################################
#   Build the static library
#

#Link with static libs
add_library(${target}-static STATIC ${rrSources})

# set version
set_target_properties(${target}-static PROPERTIES SOVERSION ${ROADRUNNER_VERSION})

add_dependencies(${target}-static all-dependencies)


if (WIN32)
    # See rrExporter for why we need STATIC_RR when building static lib.
    set_target_properties(${target}-static
            PROPERTIES COMPILE_FLAGS -DSTATIC_RR)

    # set msvc runtime
    if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        set_property(TARGET ${target}-static PROPERTY
                MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
    elseif (${CMAKE_BUILD_TYPE} STREQUAL "Release")
        set_property(TARGET ${target}-static PROPERTY
                MSVC_RUNTIME_LIBRARY "MultiThreaded")
    else ()
        message(FATAL_ERROR "Only Debug and Release configurations are supported")
    endif ()

endif ()


#  I got a successful compile with only libsbml namespace.
#  Leave it like this until you have a reason to uncommment
set_property(TARGET ${target}-static
        PROPERTY COMPILE_DEFINITIONS
        LIBSBML_USE_CPP_NAMESPACE
        STATIC_RR
        #        LIBSBML_STATIC
        #        LIBLAX_STATIC
        #        STATIC_LIBSTRUCT
        #        STATIC_PUGI
        #        STATIC_NLEQ1
        #        STATIC_NLEQ2
        #        POCO_STATIC
        )

# Order matters for linux builds. Do not change.
set(LINK_LIBRARIES
        ${RR_LIBSTRUCT_STATIC_LIBRARY}
        ${LIBSBML_STATIC_LIBRARY}
        ${SUNDIALS_STATIC_LIBRARIES}
        ${NLEQ2_STATIC_LIBRARY}
        ${NLEQ1_STATIC_LIBRARY}
        ${CLAPACK_STATIC_LIBRARIES}
        ${POCO_STATIC_LIBRARIES}
        ${LLVM_LIBRARIES}
        ${ZLIB_STATIC_LIBRARY}
        ${LIBBZ2_STATIC_LIBRARY}
        ${LIBXML2_STATIC_LIBRARY}

        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMAArch64AsmParser.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMAArch64AsmPrinter.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMAArch64CodeGen.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMAArch64Desc.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMAArch64Disassembler.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMAArch64Info.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMAArch64Utils.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMAMDGPUAsmParser.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMAMDGPUAsmPrinter.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMAMDGPUCodeGen.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMAMDGPUDesc.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMAMDGPUDisassembler.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMAMDGPUInfo.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMAMDGPUUtils.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMARMAsmParser.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMARMAsmPrinter.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMARMCodeGen.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMARMDesc.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMARMDisassembler.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMARMInfo.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMARMUtils.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMAnalysis.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMAsmParser.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMAsmPrinter.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMBPFAsmParser.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMBPFAsmPrinter.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMBPFCodeGen.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMBPFDesc.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMBPFDisassembler.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMBPFInfo.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMBinaryFormat.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMBitReader.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMBitWriter.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMCodeGen.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMCore.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMCoroutines.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMCoverage.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMDebugInfoCodeView.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMDebugInfoDWARF.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMDebugInfoMSF.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMDebugInfoPDB.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMDemangle.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMDlltoolDriver.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMExecutionEngine.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMFuzzMutate.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMGlobalISel.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMHexagonAsmParser.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMHexagonCodeGen.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMHexagonDesc.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMHexagonDisassembler.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMHexagonInfo.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMIRReader.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMInstCombine.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMInstrumentation.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMInterpreter.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMLTO.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMLanaiAsmParser.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMLanaiAsmPrinter.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMLanaiCodeGen.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMLanaiDesc.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMLanaiDisassembler.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMLanaiInfo.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMLibDriver.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMLineEditor.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMLinker.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMMC.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMMCDisassembler.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMMCJIT.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMMCParser.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMMIRParser.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMMSP430AsmPrinter.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMMSP430CodeGen.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMMSP430Desc.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMMSP430Info.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMMipsAsmParser.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMMipsAsmPrinter.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMMipsCodeGen.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMMipsDesc.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMMipsDisassembler.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMMipsInfo.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMNVPTXAsmPrinter.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMNVPTXCodeGen.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMNVPTXDesc.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMNVPTXInfo.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMObjCARCOpts.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMObject.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMObjectYAML.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMOption.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMOrcJIT.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMPasses.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMPowerPCAsmParser.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMPowerPCAsmPrinter.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMPowerPCCodeGen.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMPowerPCDesc.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMPowerPCDisassembler.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMPowerPCInfo.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMProfileData.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMRuntimeDyld.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMScalarOpts.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMSelectionDAG.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMSparcAsmParser.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMSparcAsmPrinter.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMSparcCodeGen.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMSparcDesc.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMSparcDisassembler.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMSparcInfo.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMSupport.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMSymbolize.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMSystemZAsmParser.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMSystemZAsmPrinter.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMSystemZCodeGen.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMSystemZDesc.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMSystemZDisassembler.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMSystemZInfo.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMTableGen.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMTarget.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMTransformUtils.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMVectorize.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMWindowsManifest.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMX86AsmParser.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMX86AsmPrinter.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMX86CodeGen.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMX86Desc.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMX86Disassembler.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMX86Info.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMX86Utils.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMXCoreAsmPrinter.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMXCoreCodeGen.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMXCoreDesc.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMXCoreDisassembler.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMXCoreInfo.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMXRay.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMipo.a
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLTO.so
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLTO.so.6
        /mnt/d/LLVM6.0.1/install-gcc7/lib/libLTO.so.6.0.1
        )

# Extra platform specific libraries
# These were not needed to actually build roadrunner binaries
# but linking the tests fails without them.
if (UNIX)
    set(LINK_LIBRARIES ${LINK_LIBRARIES} pthread dl m icui18n icuuc lzma)
elseif (WIN32)

elseif (APPLE)

endif ()


# make available to parent
set(LINK_LIBRARIES ${LINK_LIBRARIES} PARENT_SCOPE)

target_link_libraries(${target}-static ${LINK_LIBRARIES})

message(STATUS "LLVM_INCLUDE_DIRS ${LLVM_INCLUDE_DIRS}")

set(INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${LIBSBML_INCLUDE_DIR}
        ${POCO_INCLUDE_DIR}
        ${RR_LIBSTRUCT_INCLUDE_DIR}
        ${CLAPACK_INCLUDE_DIR}
        ${NLEQ1_INCLUDE_DIR}
        ${NLEQ2_INCLUDE_DIR}
        ${SUNDIALS_INCLUDE_DIR}
        ${LLVM_INCLUDE_DIRS}
        )
set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} PARENT_SCOPE)
target_include_directories(${target}-static PRIVATE ${INCLUDE_DIRECTORIES})


if (BUILD_SHARED_LIBS)

    add_library(${target} SHARED ${rrSources})
    set_target_properties(${target} PROPERTIES SOVERSION ${ROADRUNNER_VERSION})
    add_dependencies(${target} all-dependencies)

    if (WIN32)
        # EXPORT_RR - on windows we want EXPORT_RR defined when compiling roadrunner
        # /EHsc (see https://docs.microsoft.com/en-us/cpp/build/reference/eh-exception-handling-model?view=vs-2019):
        #   - EH = Exception Handling
        #   - s = Enables standard C++ stack unwinding
        #   - c =  assumes that functions declared as extern "C" never throw a C++ exception.
        add_definitions(-DEXPORT_RR /EHsc)

        # set msvc runtime
        if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
            set_property(TARGET ${target} PROPERTY
                    MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL")
        elseif (${CMAKE_BUILD_TYPE} STREQUAL "Release")
            set_property(TARGET ${target} PROPERTY
                    MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
        else ()
            message(FATAL_ERROR "Only Debug and Release configurations are supported")
        endif ()


    endif ()
    set_property(TARGET ${target}
            PROPERTY COMPILE_DEFINITIONS
            EXPORT_RR
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

    if (UNIX)
        list(APPEND LINK_LIBRARIES "pthread" "dl")
    elseif (WIN32)
        list(APPEND LINK_LIBRARIES "ws2_32" "iphlpapi")
    elseif (APPLE)
        # sad face =[
    endif ()

    target_include_directories(${target} PRIVATE ${INCLUDE_DIRECTORIES})
    target_link_libraries(${target} PRIVATE ${LINK_LIBRARIES})

    if (${INSTALL_CXX_API})
        install(TARGETS ${target}
                RUNTIME DESTINATION bin COMPONENT rr_core
                LIBRARY DESTINATION lib COMPONENT rr_core
                ARCHIVE DESTINATION lib COMPONENT rr_core
                )

        if (${BUILD_PYTHON})
            install(TARGETS roadrunner DESTINATION ${PYTHON_PACKAGE_DEST_DIR})
        endif ()
    endif ()


endif (BUILD_SHARED_LIBS)





















