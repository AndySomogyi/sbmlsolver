add_test_executable(llvm_tests_LLVMModelDataSymbolsTests test_targets LLVMModelDataSymbolsTests.cpp)
add_test_executable(llvm_tests_MCJitTests test_targets MCJitTests.cpp JitTests.h)
add_test_executable(llvm_tests_LLJitTests test_targets LLJitTests.cpp JitTests.h)
add_test_executable(llvm_tests_MCJitExternalFunctionTests test_targets MCJitExternalFunctionTests.cpp)
add_test_executable(llvm_tests_LLJitExternalFunctionTests test_targets LLJitMapFunctionsToJitSymbolsTests.cpp)
add_test_executable(llvm_tests_RandomTests test_targets RandomTests.cpp)
add_test_executable(
        llvm_tests_EvalInitialConditionsTests test_targets
        EvalInitialConditionsTests.cpp
        LLVMGeneratedFunctionTests.h
        LLVMGeneratedFunctionTests.cpp

)
add_test_executable(llvm_tests_JitFactoryTests test_targets JitFactoryTests.cpp)

#add_executable(BuildAModel BuildAModel.cpp)
#target_link_libraries(BuildAModel PRIVATE roadrunner-static gtest gtest_main)
#target_include_directories(BuildAModel PRIVATE roadrunner-static)

set(test_targets ${test_targets} PARENT_SCOPE)
