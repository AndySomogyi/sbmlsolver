//
// Created by Ciaran on 27/10/2021.
//

#include "llvm/MCJit.h"
#include "gtest/gtest.h"
#include "TestModelFactory.h"
#include "rrLogger.h"
#include "rrRoadRunnerOptions.h"
#include "MapFunctionsToJitSymbolsTests.h"

using namespace rr;
using namespace rrllvm;


class MCJitExternalFunctionTests : public MapFunctionsToJitSymbolsTests {
public:
    MCJitExternalFunctionTests()
            : MapFunctionsToJitSymbolsTests(std::make_unique<MCJit>(LoadSBMLOptions().modelGeneratorOpt)) {};
};

TEST_F(MCJitExternalFunctionTests, CheckLibFuncPow) {
    CheckLibFuncPow();
}

TEST_F(MCJitExternalFunctionTests, CheckLibFuncFabs) {
    CheckLibFuncFabs();
}

TEST_F(MCJitExternalFunctionTests, CheckLibFuncAcos) {
    CheckLibFuncAcos();
}

TEST_F(MCJitExternalFunctionTests, CheckLibFuncAsin) {
    CheckLibFuncAsin();
}

TEST_F(MCJitExternalFunctionTests, CheckLibFuncAtan) {
    CheckLibFuncAtan();
}

TEST_F(MCJitExternalFunctionTests, CheckLibFuncCeil) {
    CheckLibFuncCeil();
}

TEST_F(MCJitExternalFunctionTests, CheckLibFuncCos) {
    CheckLibFuncCos();
}

TEST_F(MCJitExternalFunctionTests, CheckLibFuncCosh) {
    CheckLibFuncCosh();
}

TEST_F(MCJitExternalFunctionTests, CheckLibFuncExp) {
    CheckLibFuncExp();
}

TEST_F(MCJitExternalFunctionTests, CheckLibFuncFloor) {
    CheckLibFuncFloor();
}

TEST_F(MCJitExternalFunctionTests, CheckLibFuncLog) {
    CheckLibFuncLog();
}

TEST_F(MCJitExternalFunctionTests, CheckLibFuncLog10) {
    CheckLibFuncLog10();
}

TEST_F(MCJitExternalFunctionTests, CheckLibFuncSin) {
    CheckLibFuncSin();
}

TEST_F(MCJitExternalFunctionTests, CheckLibFuncSinh) {
    CheckLibFuncSinh();
}

TEST_F(MCJitExternalFunctionTests, CheckLibFuncTan) {
    CheckLibFuncTan();
}

TEST_F(MCJitExternalFunctionTests, CheckLibFuncTanh) {
    CheckLibFuncTanh();
}

TEST_F(MCJitExternalFunctionTests, CheckLibFuncFmod) {
    CheckLibFuncFmod();
}

TEST_F(MCJitExternalFunctionTests, CheckGlobalMapping_arccotFnTy) {
    CheckGlobalMapping_arccotFnTy();
}

TEST_F(MCJitExternalFunctionTests, CheckGlobalMapping_rr_arccot_negzeroFnTy) {
    CheckGlobalMapping_rr_arccot_negzeroFnTy();
}

TEST_F(MCJitExternalFunctionTests, CheckGlobalMapping_arccothFnTy) {
    CheckGlobalMapping_arccothFnTy();
}

TEST_F(MCJitExternalFunctionTests, CheckGlobalMapping_arccscFnTy) {
    CheckGlobalMapping_arccscFnTy();
}

TEST_F(MCJitExternalFunctionTests, CheckGlobalMapping_arccschFnTy) {
    CheckGlobalMapping_arccschFnTy();
}

TEST_F(MCJitExternalFunctionTests, CheckGlobalMapping_arcsecFnTy) {
    CheckGlobalMapping_arcsecFnTy();
}

TEST_F(MCJitExternalFunctionTests, CheckGlobalMapping_arcsechFnTy) {
    CheckGlobalMapping_arcsechFnTy();
}

TEST_F(MCJitExternalFunctionTests, CheckGlobalMapping_cotFnTy) {
    CheckGlobalMapping_cotFnTy();
}

TEST_F(MCJitExternalFunctionTests, CheckGlobalMapping_cothFnTy) {
    CheckGlobalMapping_cothFnTy();
}

TEST_F(MCJitExternalFunctionTests, CheckGlobalMapping_cscFnTy) {
    CheckGlobalMapping_cscFnTy();
}

TEST_F(MCJitExternalFunctionTests, CheckGlobalMapping_cschFnTy) {
    CheckGlobalMapping_cschFnTy();
}

TEST_F(MCJitExternalFunctionTests, CheckGlobalMapping_rr_factorialiFnTy) {
    CheckGlobalMapping_rr_factorialiFnTy();
}

TEST_F(MCJitExternalFunctionTests, CheckGlobalMapping_rr_factorialdFnTy) {
    CheckGlobalMapping_rr_factorialdFnTy();
}

TEST_F(MCJitExternalFunctionTests, CheckGlobalMapping_rr_logdFnTy) {
    CheckGlobalMapping_rr_logdFnTy();
}

TEST_F(MCJitExternalFunctionTests, CheckGlobalMapping_rr_rootdFnTy) {
    CheckGlobalMapping_rr_rootdFnTy();
}

TEST_F(MCJitExternalFunctionTests, CheckGlobalMapping_secFnTy) {
    CheckGlobalMapping_secFnTy();
}

TEST_F(MCJitExternalFunctionTests, CheckGlobalMapping_sechFnTy) {
    CheckGlobalMapping_sechFnTy();
}

TEST_F(MCJitExternalFunctionTests, CheckGlobalMapping_arccoshFnTy) {
    CheckGlobalMapping_arccoshFnTy();
}

TEST_F(MCJitExternalFunctionTests, CheckGlobalMapping_arcsinhFnTy) {
    CheckGlobalMapping_arcsinhFnTy();
}

TEST_F(MCJitExternalFunctionTests, CheckGlobalMapping_arctanhFnTy) {
    CheckGlobalMapping_arctanhFnTy();
}

TEST_F(MCJitExternalFunctionTests, CheckGlobalMapping_quotientFnTy) {
    CheckGlobalMapping_quotientFnTy();
}

TEST_F(MCJitExternalFunctionTests, CheckGlobalMapping_rr_maxFnTy) {
    CheckGlobalMapping_rr_maxFnTy();
}

TEST_F(MCJitExternalFunctionTests, CheckGlobalMapping_rr_minFnTy) {
    CheckGlobalMapping_rr_minFnTy();
}


TEST_F(MCJitExternalFunctionTests, CheckGlobalMapping_rr_csr_matrix_get_nz) {
    CheckGlobalMapping_rr_csr_matrix_get_nz();
}

TEST_F(MCJitExternalFunctionTests, CheckGlobalMapping_rr_csr_matrix_set_nz) {
    CheckGlobalMapping_rr_csr_matrix_set_nz();
}



