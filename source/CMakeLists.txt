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


############################################################
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
        #todo pull test files out of main library sources.
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
    #cl   /TP -DBUILD_LLVM -DCRT_NONSTDC_NO_DEPRECATE -DLIBLAX_STATIC -DLIBSBML_STATIC -DLIBSBML_USE_CPP_NAMESPACE -DPOCO_NO_AUTOMATIC_LIBS -DPOCO_STATIC -DSTATIC_LIBSTRUCT -DSTATIC_NLEQ1 -DSTATIC_NLEQ2 -DSTATIC_PUGI -DSTATIC_RR -D_CRT_SECURE_NO_WARNINGS -ID:\rrbuild\roadrunner\source -ID:\rrbuild\roadrunner\third_party\nleq1\include -ID:\rrbuild\roadrunner\third_party\nleq2\include -ID:\rrbuild\roadrunner\third_party\clapack3.2.1\include -ID:\rrbuild\roadrunner\third_party\libSBML-5.18.1-experimental-Source\include -ID:\rrbuild\roadrunner\third_party\rr-libstruct\include -ID:\rrbuild\roadrunner\third_party\poco_1.5.3\include -ID:\rrbuild\roadrunner\third_party\sundials\include -ID:\rrbuild\roadrunner\third_party\unit_test -ID:\LLVM6.0.1\install-msvc-with-extra-flags\include /DWIN32 /D_WINDOWS /GR /EHsc /O2 /Ob2 /DNDEBUG -MT   /wd4251 /wd4018 /wd4996 /EHsc -DSTATIC_RR -ID:\LLVM6.0.1\install-msvc-with-extra-flags\include /DWIN32 /D_WINDOWS   /Zc:inline /Zc:strictStrings /Oi /Zc:rvalueCast /W4 -wd4141 -wd4146 -wd4180 -wd4244 -wd4258 -wd4267 -wd4291 -wd4345 -wd4351 -wd4355 -wd4456 -wd4457 -wd4458 -wd4459 -wd4503 -wd4624 -wd4722 -wd4800 -wd4100 -wd4127 -wd4512 -wd4505 -wd4610 -wd4510 -wd4702 -wd4245 -wd4706 -wd4310 -wd4701 -wd4703 -wd4389 -wd4611 -wd4805 -wd4204 -wd4577 -wd4091 -wd4592 -wd4319 -wd4324 -w14062 -we4238 /MD /O2 /Ob2 /DNDEBUG  /EHsc /GR -D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_NONSTDC_NO_WARNINGS -D_SCL_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS -DUNICODE -D_UNICODE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS /FoCMakeFiles\roadrunner-static.dir\llvm\Random.cpp.obj /FdCMakeFiles\roadrunner-static.dir\roadrunner-static.pdb /FS -c D:\rrbuild\roadrunner\source\llvm\Random.cpp
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
    # install the rr_support if building legacy c
    add_subdirectory(rr_support)
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
    SetTargetMSVCRuntimeStatic(${target}-static)

    # We have a problem where a librry (maybe llvm) is enforcing their
    #  compile flags on us and overriding the msvc runtime.
    get_target_property(static_flags ${target}-static "CMAKE_CXX_FLAGS")
    MESSAGE(STATUS "static_flags ${static_flags}")
#    foreach(flag)

endif ()


set_property(TARGET ${target}-static
        PROPERTY COMPILE_DEFINITIONS
        LIBSBML_USE_CPP_NAMESPACE
        STATIC_RR
        LIBSBML_STATIC
        LIBLAX_STATIC
        STATIC_LIBSTRUCT
        STATIC_PUGI
        STATIC_NLEQ1
        STATIC_NLEQ2
        POCO_STATIC
        )

if (WIN32)
    #    set_target_properties(${target}-static PROPERTIES LINK_FLAGS "/WHOLEARCHIVE")
    #    target_link_options(${target}-static PUBLIC /WHOLE_ARCHIVE)
endif ()
# Order matters for linux builds. Do not change.
#set(LINK_LIBRARIES
#        ${RR_LIBSTRUCT_STATIC_LIBRARY}
#        #        ${LIBSBML_STATIC_LIBRARY}
#        ${SUNDIALS_STATIC_LIBRARIES}
#        ${NLEQ2_STATIC_LIBRARY}
#        ${NLEQ1_STATIC_LIBRARY}
#        ${CLAPACK_STATIC_LIBRARIES}
#        ${POCO_STATIC_LIBRARIES}
#        #        ${LLVM_LIBRARIES}
#        #        ${ZLIB_STATIC_LIBRARY}
#        #        ${LIBBZ2_STATIC_LIBRARY}
#        #        ${LIBXML2_STATIC_LIBRARY}
#        #        ${LIBICONV_STATIC_LIBRARY}
#        #        ${ZLIB_STATIC_LIBRARY}
#        )
#
## make available to parent
#set(LINK_LIBRARIES ${LINK_LIBRARIES} PARENT_SCOPE)
#

# Extra platform specific libraries
# These were not needed to actually build roadrunner binaries
# but linking the tests fails without them.
if (UNIX)
    #    icui18n icuuc
    set(LINK_LIBRARIES ${LINK_LIBRARIES} pthread dl m lzma)
elseif (WIN32)
    set(LINK_LIBRARIES ${LINK_LIBRARIES} Ws2_32)

elseif (APPLE)

endif ()

target_link_libraries(${target}-static ${LINK_LIBRARIES})

set(ROADRUNNER_INTERNAL_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
set(ROADRUNNER_INTERNAL_INCLUDE_DIRS ${ROADRUNNER_INTERNAL_INCLUDE_DIRS} PARENT_SCOPE)

set(INCLUDE_DIRECTORIES
        ${ROADRUNNER_INTERNAL_INCLUDE_DIRS}
        ${DEPENDENCY_INCLUDE_DIRS}
        ${LLVM_INCLUDE_DIRS}
        )
#        ${ROADRUNNER_INTERNAL_INCLUDE_DIRS}
#        ${LIBSBML_INCLUDE_DIR}
#        ${POCO_INCLUDE_DIR}
#        ${RR_LIBSTRUCT_INCLUDE_DIR}
#        ${CLAPACK_INCLUDE_DIR}
#        ${NLEQ1_INCLUDE_DIR}
#        ${NLEQ2_INCLUDE_DIR}
#        ${SUNDIALS_INCLUDE_DIR}
#        ${LLVM_INCLUDE_DIRS}
#        ${gtest_SOURCE_DIR}
#        )
set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} PARENT_SCOPE)
target_include_directories(${target}-static PRIVATE ${INCLUDE_DIRECTORIES})

# turn off shared zlib for roadruner (Ciaran welsh)
#set(BUILD_SHARED_LIBS OFF)
if (BUILD_SHARED_LIBS)

    add_library(${target} SHARED ${rrSources})
    set_target_properties(${target} PROPERTIES SOVERSION ${ROADRUNNER_VERSION})
    #    add_dependencies(${target} all-dependencies)

    if (WIN32)
        # EXPORT_RR - on windows we want EXPORT_RR defined when compiling roadrunner
        # /EHsc (see https://docs.microsoft.com/en-us/cpp/build/reference/eh-exception-handling-model?view=vs-2019):
        #   - EH = Exception Handling
        #   - s = Enables standard C++ stack unwinding
        #   - c =  assumes that functions declared as extern "C" never throw a C++ exception.
        #        add_definitions(-DEXPORT_RR /EHsc)
        #todo find where LLVM_FLAGS are being added to CXX_FLAGS. We need to
        # add this after since right now it is just being overriden by LLVM_flags.

        target_link_options(${target} PUBLIC /WHOLE_ARCHIVE)

        # set msvc runtime
        SetTargetMSVCRuntimeDll(${target})

        get_target_property(x ${target} MSVC_RUNTIME_LIBRARY)
        message(STATUS "x is ' ${x}")


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
    target_link_libraries(${target} PRIVATE
            blas
            f2c
            lapack
            libsbml-static
            libiconv-static
            libLZMA-static
            libxml2-static
            zlibstatic
            nleq1-static
            nleq2-static
            PocoFoundation
            PocoNet
            PocoUtil
            PocoXML
            PocoZip
            PocoJSON
            rr-libstruct-static
            sundials_cvode_static
            sundials_cvodes_static
            sundials_ida_static
            sundials_idas_static
            sundials_kinsol_static
            sundials_nvecserial_static
            unit_test-static
            ${LLVM_LIBRARIES}
            )

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





















