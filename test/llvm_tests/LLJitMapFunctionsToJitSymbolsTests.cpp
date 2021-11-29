//
// Created by Ciaran on 27/10/2021.
//

#include "llvm/LLJit.h"
#include "gtest/gtest.h"
#include "TestModelFactory.h"
#include "rrLogger.h"
#include "rrRoadRunnerOptions.h"
#include "MapFunctionsToJitSymbolsTests.h"

using namespace rr;
using namespace rrllvm;


class LLJitMapFunctionsToJitSymbolsTests : public MapFunctionsToJitSymbolsTests {
public:
    LLJitMapFunctionsToJitSymbolsTests()
        : MapFunctionsToJitSymbolsTests(std::make_unique<LLJit>(LoadSBMLOptions().modelGeneratorOpt)){};
};

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckLibFuncPow) {
    LLJit* llJit = (LLJit*)jit.get();
    std::cout << llJit->emitToString() << std::endl;
    llJit->addIRModule();
    CheckLibFuncPow();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckLibFuncFabs){
    CheckLibFuncFabs();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckLibFuncAcos){
    CheckLibFuncAcos();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckLibFuncAsin){
    CheckLibFuncAsin();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckLibFuncAtan){
    CheckLibFuncAtan();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckLibFuncCeil){
    CheckLibFuncCeil();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckLibFuncCos){
    CheckLibFuncCos();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckLibFuncCosh){
    CheckLibFuncCosh();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckLibFuncExp){
    CheckLibFuncExp();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckLibFuncFloor){
    CheckLibFuncFloor();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckLibFuncLog){
    CheckLibFuncLog();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckLibFuncLog10){
    CheckLibFuncLog10();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckLibFuncSin){
    CheckLibFuncSin();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckLibFuncSinh){
    CheckLibFuncSinh();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckLibFuncTan){
    CheckLibFuncTan();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckLibFuncTanh){
    CheckLibFuncTanh();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckLibFuncFmod){
    CheckLibFuncFmod();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckGlobalMapping_arccotFnTy){
    CheckGlobalMapping_arccotFnTy();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckGlobalMapping_rr_arccot_negzeroFnTy){
    CheckGlobalMapping_rr_arccot_negzeroFnTy();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckGlobalMapping_arccothFnTy){
    CheckGlobalMapping_arccothFnTy();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckGlobalMapping_arccscFnTy){
    CheckGlobalMapping_arccscFnTy();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckGlobalMapping_arccschFnTy){
    CheckGlobalMapping_arccschFnTy();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckGlobalMapping_arcsecFnTy){
    CheckGlobalMapping_arcsecFnTy();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckGlobalMapping_arcsechFnTy){
    CheckGlobalMapping_arcsechFnTy();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckGlobalMapping_cotFnTy){
    CheckGlobalMapping_cotFnTy();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckGlobalMapping_cothFnTy){
    CheckGlobalMapping_cothFnTy();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckGlobalMapping_cscFnTy){
    CheckGlobalMapping_cscFnTy();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckGlobalMapping_cschFnTy){
    CheckGlobalMapping_cschFnTy();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckGlobalMapping_rr_factorialiFnTy){
    CheckGlobalMapping_rr_factorialiFnTy();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckGlobalMapping_rr_factorialdFnTy){
    CheckGlobalMapping_rr_factorialdFnTy();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckGlobalMapping_rr_logdFnTy){
    CheckGlobalMapping_rr_logdFnTy();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckGlobalMapping_rr_rootdFnTy){
    CheckGlobalMapping_rr_rootdFnTy();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckGlobalMapping_secFnTy){
    CheckGlobalMapping_secFnTy();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckGlobalMapping_sechFnTy){
    CheckGlobalMapping_sechFnTy();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckGlobalMapping_arccoshFnTy){
    CheckGlobalMapping_arccoshFnTy();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckGlobalMapping_arcsinhFnTy){
    CheckGlobalMapping_arcsinhFnTy();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckGlobalMapping_arctanhFnTy){
    CheckGlobalMapping_arctanhFnTy();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckGlobalMapping_quotientFnTy){
    CheckGlobalMapping_quotientFnTy();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckGlobalMapping_rr_maxFnTy){
    CheckGlobalMapping_rr_maxFnTy();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckGlobalMapping_rr_minFnTy){
    CheckGlobalMapping_rr_minFnTy();
}


TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckGlobalMapping_rr_csr_matrix_get_nz) {
    CheckGlobalMapping_rr_csr_matrix_get_nz();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, CheckGlobalMapping_rr_csr_matrix_set_nz) {
    CheckGlobalMapping_rr_csr_matrix_set_nz();
}

TEST_F(LLJitMapFunctionsToJitSymbolsTests, OnlyBuild) {
    RoadRunner rr(Brown2004().str());

}



