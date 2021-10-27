//
// Created by Ciaran on 27/10/2021.
//

#include "llvm/MCJit.h"
#include "gtest/gtest.h"
#include "TestModelFactory.h"
#include "rrLogger.h"
#include "rrRoadRunnerOptions.h"

using namespace rr;
using namespace rrllvm;

/**
 * @brief Tests functions that are pulled into llvm IR from either the
 * C standard library or from sbml directly.
 * @details historically these were handled by the functions createCLibraryFunctions
 * and addGlobalMappings which (to me) are not particularly descriptive. For a better
 * OOP design, these may be pulled out and put into an ExternalFunctions class. This
 * needs to be independent of which llvm backend is in use.
 *
 * todo The current tests only test the MCJit engine. These tests will need to change
 *  once the orc jit is appropriately developed.
 */
class ExternalFunctionTests : public ::testing::Test {
public:
    ExternalFunctionTests() = default;

};


// double pow(double x, double y)
TEST_F(ExternalFunctionTests, LibFuncPow) {
    double expected = 16;
    std::string funcName = "pow";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    powFnTy fnPtr = (powFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_EQ(expected, fnPtr(2, 4));
}


TEST_F(ExternalFunctionTests, CheckLibFuncFabs) {
    double expected = 5.3;
    std::string funcName = "fabs";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    fabsFnTy fnPtr = (fabsFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_EQ(expected, fnPtr(-5.3));
}

TEST_F(ExternalFunctionTests, CheckLibFuncAcos) {
    // Inverse of cos(-0.50) = 2.09 in radians
    double expected = 2.0943951;
    std::string funcName = "acos";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    acosFnTy fnPtr = (acosFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(-0.5), 1e-3);
}

TEST_F(ExternalFunctionTests, CheckLibFuncAsin) {
    // Inverse of sin(-0.50) = -0.52 in radians
    double expected = -0.523598776;
    std::string funcName = "asin";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    asinFnTy fnPtr = (asinFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(-0.5), 1e-7);
}

TEST_F(ExternalFunctionTests, CheckLibFuncAtan) {
    double expected = -0.463647609;
    std::string funcName = "atan";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    atanFnTy fnPtr = (atanFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(-0.5), 1e-7);
}

TEST_F(ExternalFunctionTests, CheckLibFuncCeil) {
    double expected = 7;
    std::string funcName = "ceil";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    ceilFnTy fnPtr = (ceilFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_EQ(expected, fnPtr(6.3));
}

TEST_F(ExternalFunctionTests, CheckLibFuncCos) {
    double expected = 0.87758256189;
    std::string funcName = "cos";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    cosFnTy fnPtr = (cosFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(-0.5), 1e-7);
}

TEST_F(ExternalFunctionTests, CheckLibFuncCosh) {
    double expected = 1.1276259652;
    std::string funcName = "cosh";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    coshFnTy fnPtr = (coshFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(-0.5), 1e-7);
}

TEST_F(ExternalFunctionTests, CheckLibFuncExp) {
    double expected = 0.60653065971;
    std::string funcName = "exp";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    expFnTy fnPtr = (expFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(-0.5), 1e-7);
}

TEST_F(ExternalFunctionTests, CheckLibFuncFloor) {
    double expected = 3;
    std::string funcName = "floor";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    floorFnTy fnPtr = (floorFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_EQ(expected, fnPtr(3.7));
}

TEST_F(ExternalFunctionTests, CheckLibFuncLog) {
    double expected = 3.4657;
    std::string funcName = "log";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    logFnTy fnPtr = (logFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(32), 1e-3);
}

TEST_F(ExternalFunctionTests, CheckLibFuncLog10) {
    double expected = 2;
    std::string funcName = "log10";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    log10FnTy fnPtr = (log10FnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(100), 1e-7);
}

TEST_F(ExternalFunctionTests, CheckLibFuncSin) {
    double expected = -0.4794255386;
    std::string funcName = "sin";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    sinFnTy fnPtr = (sinFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(-0.5), 1e-7);
}

TEST_F(ExternalFunctionTests, CheckLibFuncSinh) {
    double expected = -0.52109530549;
    std::string funcName = "sinh";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    sinhFnTy fnPtr = (sinhFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(-0.5), 1e-7);
}

TEST_F(ExternalFunctionTests, CheckLibFuncTan) {
    double expected = -0.54630248984;
    std::string funcName = "tan";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    tanFnTy fnPtr = (tanFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(-0.5), 1e-7);
}

TEST_F(ExternalFunctionTests, CheckLibFuncTanh) {
    double expected = -0.46211715726;
    std::string funcName = "tanh";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    tanhFnTy fnPtr = (tanhFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(-0.5), 1e-7);
}

TEST_F(ExternalFunctionTests, CheckLibFuncFmod) {
    double expected = 1;
    std::string funcName = "fmod";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    fmodFnTy fnPtr = (fmodFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_EQ(expected, fnPtr(5, 2));
}

TEST_F(ExternalFunctionTests, CheckGlobalMapping_arccotFnTy) {
    double expected = -1.1071487177940904;
    std::string funcName = "arccot";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    arccotFnTy fnPtr = (arccotFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(-0.5), 1e-5);
}

TEST_F(ExternalFunctionTests, CheckGlobalMapping_rr_arccot_negzeroFnTy) {
    double expected = -1.1071487177940904;
    std::string funcName = "rr_arccot_negzero";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    rr_arccot_negzeroFnTy fnPtr = (rr_arccot_negzeroFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(
            funcName);
    ASSERT_NEAR(expected, fnPtr(-0.5), 1e-5);
}

TEST_F(ExternalFunctionTests, CheckGlobalMapping_arccothFnTy) {
    double expected = 0.423649;
    std::string funcName = "arccoth";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    arccothFnTy fnPtr = (arccothFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(2.5), 1e-5);
}

TEST_F(ExternalFunctionTests, CheckGlobalMapping_arccscFnTy) {
    double expected = 0.41151684606748806;
    std::string funcName = "arccsc";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    arccscFnTy fnPtr = (arccscFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(2.5), 1e-5);
}

TEST_F(ExternalFunctionTests, CheckGlobalMapping_arccschFnTy) {
    double expected = -1.4436354751788099;
    std::string funcName = "arccsch";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    arccschFnTy fnPtr = (arccschFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(-0.5), 1e-5);
}

TEST_F(ExternalFunctionTests, CheckGlobalMapping_arcsecFnTy) {
    double expected = 1.1592794807274085;
    std::string funcName = "arcsec";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    arcsecFnTy fnPtr = (arcsecFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(2.5), 1e-5);
}

TEST_F(ExternalFunctionTests, CheckGlobalMapping_arcsechFnTy) {
    double expected = 1.31696;
    std::string funcName = "arcsech";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    arcsechFnTy fnPtr = (arcsechFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(0.5), 1e-5);
}

TEST_F(ExternalFunctionTests, CheckGlobalMapping_cotFnTy) {
    double expected = -1.830487721712452;
    std::string funcName = "cot";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    cotFnTy fnPtr = (cotFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);

    ASSERT_NEAR(expected, fnPtr(-0.5), 1e-5);
}

TEST_F(ExternalFunctionTests, CheckGlobalMapping_cothFnTy) {
    double expected = -2.1639534137386525;
    std::string funcName = "coth";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    cothFnTy fnPtr = (cothFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);

    ASSERT_NEAR(expected, fnPtr(-0.5), 1e-5);
}

TEST_F(ExternalFunctionTests, CheckGlobalMapping_cscFnTy) {
    double expected = -2.0858296429334882;
    std::string funcName = "csc";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    cscFnTy fnPtr = (cscFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);

    ASSERT_NEAR(expected, fnPtr(-0.5), 1e-5);
}

TEST_F(ExternalFunctionTests, CheckGlobalMapping_cschFnTy) {
    double expected = 0.16528366985509557;
    std::string funcName = "csch";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    cschFnTy fnPtr = (cschFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);

    ASSERT_NEAR(expected, fnPtr(2.5), 1e-5);
}

TEST_F(ExternalFunctionTests, CheckGlobalMapping_rr_factorialiFnTy) {
    double expected = 120;
    std::string funcName = "rr_factoriali";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    rr_factorialiFnTy fnPtr = (rr_factorialiFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(5), 1e-5);
}

TEST_F(ExternalFunctionTests, CheckGlobalMapping_rr_factorialdFnTy) {
    double expected = 24;
    std::string funcName = "rr_factoriald";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    rr_factorialdFnTy fnPtr = (rr_factorialdFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(
            funcName);
    ASSERT_NEAR(expected, fnPtr(4), 1e-5);
}

TEST_F(ExternalFunctionTests, CheckGlobalMapping_rr_logdFnTy) {
    double expected = 0.15051499783199057;
    std::string funcName = "rr_logd";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    rr_logdFnTy fnPtr = (rr_logdFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(100, 2), 1e-5);
}

TEST_F(ExternalFunctionTests, CheckGlobalMapping_rr_rootdFnTy) {
    double expected = 0.49699388728942728;
    std::string funcName = "rr_rootd";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    rr_rootdFnTy fnPtr = (rr_rootdFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(1024), 1e-5);
}

TEST_F(ExternalFunctionTests, CheckGlobalMapping_secFnTy) {
    double expected = 1.139493927324549;
    std::string funcName = "sec";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    secFnTy fnPtr = (secFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);

    ASSERT_NEAR(expected, fnPtr(-0.5), 1e-5);
}

TEST_F(ExternalFunctionTests, CheckGlobalMapping_sechFnTy) {
    double expected = 0.88681888397007402;
    std::string funcName = "sech";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    sechFnTy fnPtr = (sechFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);

    ASSERT_NEAR(expected, fnPtr(-0.5), 1e-5);
}

TEST_F(ExternalFunctionTests, CheckGlobalMapping_arccoshFnTy) {
    double expected = 1.5667992369724109;
    std::string funcName = "arccosh";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    arccoshFnTy fnPtr = (arccoshFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(2.5), 1e-5);
}

TEST_F(ExternalFunctionTests, CheckGlobalMapping_arcsinhFnTy) {
    double expected = -0.48121182505960347;
    std::string funcName = "arcsinh";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    arcsinhFnTy fnPtr = (arcsinhFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(-0.5), 1e-5);
}

TEST_F(ExternalFunctionTests, CheckGlobalMapping_arctanhFnTy) {
    double expected = -0.54930614433405489;
    std::string funcName = "arctanh";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    arctanhFnTy fnPtr = (arctanhFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(-0.5), 1e-5);
}

TEST_F(ExternalFunctionTests, CheckGlobalMapping_quotientFnTy) {
    double expected = 5;
    std::string funcName = "quotient";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    quotientFnTy fnPtr = (quotientFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(10, 2), 1e-5);
}

TEST_F(ExternalFunctionTests, CheckGlobalMapping_rr_maxFnTy) {
    double expected = 4;
    std::string funcName = "rr_max";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    rr_maxFnTy fnPtr = (rr_maxFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(3, 4), 1e-5);
}

TEST_F(ExternalFunctionTests, CheckGlobalMapping_rr_minFnTy) {
    double expected = 3;
    std::string funcName = "rr_min";
    LoadSBMLOptions opt;
    MCJit mcJit(opt.modelGeneratorOpt);
    rr_minFnTy fnPtr = (rr_minFnTy) mcJit.getExecutionEngineNonOwning()->getPointerToNamedFunction(funcName);
    ASSERT_NEAR(expected, fnPtr(3, 4), 1e-5);
}
