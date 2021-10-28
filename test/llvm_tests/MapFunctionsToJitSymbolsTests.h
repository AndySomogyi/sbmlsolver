//
// Created by Ciaran on 27/10/2021.
//

#ifndef ROADRUNNER_MAPFUNCTIONSTOJITSYMBOLSTESTS_H
#define ROADRUNNER_MAPFUNCTIONSTOJITSYMBOLSTESTS_H

#include <rrSparse.h>
#include "Jit.h"

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
class MapFunctionsToJitSymbolsTests : public ::testing::Test {
public:
    MapFunctionsToJitSymbolsTests() = default;

    explicit MapFunctionsToJitSymbolsTests(std::unique_ptr<Jit> jitPtr)
        : jit(std::move(jitPtr)){};

    void setJitEngine(std::unique_ptr<Jit> jitPtr){
        jit = std::move(jitPtr);
    }

    template<typename FnType, typename ReturnValueType, typename... InputValueType>
    void checkFunctionCalled(Jit *jit, const std::string &name, ReturnValueType expected, InputValueType... fnArgs) {
        FnType fnPtr = (FnType) jit->getFunctionAddress(name);
        ReturnValueType actual = fnPtr(fnArgs...);
        ASSERT_NEAR(expected, actual, 1e-5);
    }

    void CheckLibFuncPow() {
        double expected = 16;
        std::string funcName = "pow";
        powFnTy fnPtr = (powFnTy) jit->getFunctionAddress(funcName);
        ASSERT_EQ(expected, fnPtr(2, 4));
    }


    void CheckLibFuncFabs() {
        double expected = 5.3;
        std::string funcName = "fabs";
        fabsFnTy fnPtr = (fabsFnTy) jit->getFunctionAddress(funcName);
        ASSERT_EQ(expected, fnPtr(-5.3));
    }

    void CheckLibFuncAcos() {
        // Inverse of cos(-0.50) = 2.09 in radians
        double expected = 2.0943951;
        std::string funcName = "acos";
        acosFnTy fnPtr = (acosFnTy) jit->getFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-3);
    }

    void CheckLibFuncAsin() {
        // Inverse of sin(-0.50) = -0.52 in radians
        double expected = -0.523598776;
        std::string funcName = "asin";
        asinFnTy fnPtr = (asinFnTy) jit->getFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-7);
    }

    void CheckLibFuncAtan() {
        double expected = -0.463647609;
        std::string funcName = "atan";
        atanFnTy fnPtr = (atanFnTy) jit->getFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-7);
    }

    void CheckLibFuncCeil() {
        double expected = 7;
        std::string funcName = "ceil";
        ceilFnTy fnPtr = (ceilFnTy) jit->getFunctionAddress(funcName);
        ASSERT_EQ(expected, fnPtr(6.3));
    }

    void CheckLibFuncCos() {
        double expected = 0.87758256189;
        std::string funcName = "cos";
        cosFnTy fnPtr = (cosFnTy) jit->getFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-7);
    }

    void CheckLibFuncCosh() {
        double expected = 1.1276259652;
        std::string funcName = "cosh";
        coshFnTy fnPtr = (coshFnTy) jit->getFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-7);
    }

    void CheckLibFuncExp() {
        double expected = 0.60653065971;
        std::string funcName = "exp";
        expFnTy fnPtr = (expFnTy) jit->getFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-7);
    }

    void CheckLibFuncFloor() {
        double expected = 3;
        std::string funcName = "floor";
        floorFnTy fnPtr = (floorFnTy) jit->getFunctionAddress(funcName);
        ASSERT_EQ(expected, fnPtr(3.7));
    }

    void CheckLibFuncLog() {
        double expected = 3.4657;
        std::string funcName = "log";
        logFnTy fnPtr = (logFnTy) jit->getFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(32), 1e-3);
    }

    void CheckLibFuncLog10() {
        double expected = 2;
        std::string funcName = "log10";
        log10FnTy fnPtr = (log10FnTy) jit->getFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(100), 1e-7);
    }

    void CheckLibFuncSin() {
        double expected = -0.4794255386;
        std::string funcName = "sin";
        sinFnTy fnPtr = (sinFnTy) jit->getFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-7);
    }

    void CheckLibFuncSinh() {
        double expected = -0.52109530549;
        std::string funcName = "sinh";
        sinhFnTy fnPtr = (sinhFnTy) jit->getFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-7);
    }

    void CheckLibFuncTan() {
        double expected = -0.54630248984;
        std::string funcName = "tan";
        tanFnTy fnPtr = (tanFnTy) jit->getFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-7);
    }

    void CheckLibFuncTanh() {
        double expected = -0.46211715726;
        std::string funcName = "tanh";
        tanhFnTy fnPtr = (tanhFnTy) jit->getFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-7);
    }

    void CheckLibFuncFmod() {
        double expected = 1;
        std::string funcName = "fmod";
        fmodFnTy fnPtr = (fmodFnTy) jit->getFunctionAddress(funcName);
        ASSERT_EQ(expected, fnPtr(5, 2));
    }

    void CheckGlobalMapping_arccotFnTy() {
        double expected = -1.1071487177940904;
        std::string funcName = "arccot";
        arccotFnTy fnPtr = (arccotFnTy) jit->getFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-5);
    }

    void CheckGlobalMapping_rr_arccot_negzeroFnTy() {
        double expected = -1.1071487177940904;
        std::string funcName = "rr_arccot_negzero";
        rr_arccot_negzeroFnTy fnPtr = (rr_arccot_negzeroFnTy) jit->getFunctionAddress(
                funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-5);
    }

    void CheckGlobalMapping_arccothFnTy() {
        double expected = 0.423649;
        std::string funcName = "arccoth";
        arccothFnTy fnPtr = (arccothFnTy) jit->getFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(2.5), 1e-5);
    }

    void CheckGlobalMapping_arccscFnTy() {
        double expected = 0.41151684606748806;
        std::string funcName = "arccsc";
        arccscFnTy fnPtr = (arccscFnTy) jit->getFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(2.5), 1e-5);
    }

    void CheckGlobalMapping_arccschFnTy() {
        double expected = -1.4436354751788099;
        std::string funcName = "arccsch";
        arccschFnTy fnPtr = (arccschFnTy) jit->getFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-5);
    }

    void CheckGlobalMapping_arcsecFnTy() {
        double expected = 1.1592794807274085;
        std::string funcName = "arcsec";
        arcsecFnTy fnPtr = (arcsecFnTy) jit->getFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(2.5), 1e-5);
    }

    void CheckGlobalMapping_arcsechFnTy() {
        double expected = 1.31696;
        std::string funcName = "arcsech";
        arcsechFnTy fnPtr = (arcsechFnTy) jit->getFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(0.5), 1e-5);
    }

    void CheckGlobalMapping_cotFnTy() {
        double expected = -1.830487721712452;
        std::string funcName = "cot";
        cotFnTy fnPtr = (cotFnTy) jit->getFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-5);
    }

    void CheckGlobalMapping_cothFnTy() {
        double expected = -2.1639534137386525;
        std::string funcName = "coth";
        cothFnTy fnPtr = (cothFnTy) jit->getFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-5);
    }

    void CheckGlobalMapping_cscFnTy() {
        double expected = -2.0858296429334882;
        std::string funcName = "csc";
        cscFnTy fnPtr = (cscFnTy) jit->getFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-5);
    }

    void CheckGlobalMapping_cschFnTy() {
        double expected = 0.16528366985509557;
        std::string funcName = "csch";
        cschFnTy fnPtr = (cschFnTy) jit->getFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(2.5), 1e-5);
    }

    void CheckGlobalMapping_rr_factorialiFnTy() {
        double expected = 120;
        std::string funcName = "rr_factoriali";
        rr_factorialiFnTy fnPtr = (rr_factorialiFnTy) jit->getFunctionAddress(
                funcName);
        ASSERT_NEAR(expected, fnPtr(5), 1e-5);
    }

    void CheckGlobalMapping_rr_factorialdFnTy() {
        double expected = 24;
        std::string funcName = "rr_factoriald";
        rr_factorialdFnTy fnPtr = (rr_factorialdFnTy) jit->getFunctionAddress(
                funcName);
        ASSERT_NEAR(expected, fnPtr(4), 1e-5);
    }

    void CheckGlobalMapping_rr_logdFnTy() {
        double expected = 0.15051499783199057;
        std::string funcName = "rr_logd";
        rr_logdFnTy fnPtr = (rr_logdFnTy) jit->getFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(100, 2), 1e-5);
    }

    void CheckGlobalMapping_rr_rootdFnTy() {
        double expected = 2;
        std::string funcName = "rr_rootd";
        rr_rootdFnTy fnPtr = (rr_rootdFnTy) jit->getFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(2, 4), 1e-5);
    }

    void CheckGlobalMapping_secFnTy() {
        double expected = 1.139493927324549;
        std::string funcName = "sec";
        secFnTy fnPtr = (secFnTy) jit->getFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-5);
    }

    void CheckGlobalMapping_sechFnTy() {
        double expected = 0.88681888397007402;
        std::string funcName = "sech";
        sechFnTy fnPtr = (sechFnTy) jit->getFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-5);
    }

    void CheckGlobalMapping_arccoshFnTy() {
        double expected = 1.5667992369724109;
        std::string funcName = "arccosh";
        arccoshFnTy fnPtr = (arccoshFnTy) jit->getFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(2.5), 1e-5);
    }

    void CheckGlobalMapping_arcsinhFnTy() {
        double expected = -0.48121182505960347;
        std::string funcName = "arcsinh";
        arcsinhFnTy fnPtr = (arcsinhFnTy) jit->getFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-5);
    }

    void CheckGlobalMapping_arctanhFnTy() {
        double expected = -0.54930614433405489;
        std::string funcName = "arctanh";
        arctanhFnTy fnPtr = (arctanhFnTy) jit->getFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-5);
    }

    void CheckGlobalMapping_quotientFnTy() {
        double expected = 5;
        std::string funcName = "quotient";
        quotientFnTy fnPtr = (quotientFnTy) jit->getFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(10, 2), 1e-5);
    }

    void CheckGlobalMapping_rr_maxFnTy() {
        double expected = 4;
        std::string funcName = "rr_max";
        rr_maxFnTy fnPtr = (rr_maxFnTy) jit->getFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(3, 4), 1e-5);
    }

    void CheckGlobalMapping_rr_minFnTy() {
        double expected = 3;
        std::string funcName = "rr_min";
        rr_minFnTy fnPtr = (rr_minFnTy) jit->getFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(3, 4), 1e-5);
    }

    void CheckGlobalMapping_rr_csr_matrix_get_nz() {
        double expected = 3;
        std::string funcName = "rr_csr_matrix_get_nz";

        csr_matrix_get_nz_FnTy csr_matrix_get_nz = (csr_matrix_get_nz_FnTy)jit->getFunctionAddress(funcName);
        ASSERT_FALSE(csr_matrix_get_nz == nullptr);
    }


    void CheckGlobalMapping_rr_csr_matrix_set_nz() {
        double expected = 3;
        std::string funcName = "rr_csr_matrix_set_nz";

        csr_matrix_set_nz_FnTy csr_matrix_set_nz = (csr_matrix_set_nz_FnTy)jit->getFunctionAddress(funcName);
        std::cout << csr_matrix_set_nz << std::endl;
        ASSERT_FALSE(csr_matrix_set_nz == nullptr);
    }


protected:

    // non owning
    std::unique_ptr<Jit> jit;
    LoadSBMLOptions opt;

};



#endif //ROADRUNNER_MAPFUNCTIONSTOJITSYMBOLSTESTS_H
