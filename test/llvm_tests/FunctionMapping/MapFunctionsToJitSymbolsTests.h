//
// Created by Ciaran on 27/10/2021.
//

#ifndef ROADRUNNER_MAPFUNCTIONSTOJITSYMBOLSTESTS_H
#define ROADRUNNER_MAPFUNCTIONSTOJITSYMBOLSTESTS_H

#include "rrSparse.h"
#include "Jit.h"

using namespace rr;
using namespace rrllvm;

/**
 * @brief Tests functions that are pulled into llvm IR from either the
 * C standard library or from sbml directly.
 * @details historically these were handled by the functions createCLibraryFunctions
 * and adds which (to me) are not particularly descriptive. For a better
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
            : jit(std::move(jitPtr)) {};

    void setJitEngine(std::unique_ptr<Jit> jitPtr) {
        jit = std::move(jitPtr);
    }

    /**
     * This works, but less clear whats going on. With unit tests, I prefer 
     * to make the code clear at the cost of more typing. 
     */
    template<typename FnType, typename ReturnValueType, typename... InputValueType>
    void checkFunctionCalled(Jit *jit, const std::string &name, ReturnValueType expected, InputValueType... fnArgs) {
        FnType fnPtr = (FnType) jit->lookupFunctionAddress(name);
        ReturnValueType actual = fnPtr(fnArgs...);
        ASSERT_NEAR(expected, actual, 1e-5);
    }

    void checkFunctionLoadsUsingModule(Jit* jit, const std::string& fnName){
        llvm::Function* fn = jit->getModuleNonOwning()->getFunction(fnName);
        if (!fn){
            rrLogCritical << "Function \"" << fnName << "\" could not be loaded";
        }
        ASSERT_TRUE(fn);


    }

    void CheckLibFuncPow() {
        double expected = 16;
        std::string funcName = "pow";
        powFnTy fnPtr = (powFnTy) jit->lookupFunctionAddress(funcName);
        ASSERT_EQ(expected, fnPtr(2, 4));
    }

    void CheckLibFuncFabs() {
        double expected = 5.3;
        std::string funcName = "fabs";
        fabsFnTy fnPtr = (fabsFnTy) jit->lookupFunctionAddress(funcName);
        ASSERT_EQ(expected, fnPtr(-5.3));
    }

    void CheckLibFuncAcos() {
        // Inverse of cos(-0.50) = 2.09 in radians
        double expected = 2.0943951;
        std::string funcName = "acos";
        acosFnTy fnPtr = (acosFnTy) jit->lookupFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-3);
    }

    void CheckLibFuncAsin() {
        // Inverse of sin(-0.50) = -0.52 in radians
        double expected = -0.523598776;
        std::string funcName = "asin";
        asinFnTy fnPtr = (asinFnTy) jit->lookupFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-7);
    }

    void CheckLibFuncAtan() {
        double expected = -0.463647609;
        std::string funcName = "atan";
        atanFnTy fnPtr = (atanFnTy) jit->lookupFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-7);
    }

    void CheckLibFuncCeil() {
        double expected = 7;
        std::string funcName = "ceil";
        ceilFnTy fnPtr = (ceilFnTy) jit->lookupFunctionAddress(funcName);
        ASSERT_EQ(expected, fnPtr(6.3));
    }

    void CheckLibFuncCos() {
        double expected = 0.87758256189;
        std::string funcName = "cos";
        cosFnTy fnPtr = (cosFnTy) jit->lookupFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-7);
    }

    void CheckLibFuncCosh() {
        double expected = 1.1276259652;
        std::string funcName = "cosh";
        coshFnTy fnPtr = (coshFnTy) jit->lookupFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-7);
    }

    void CheckLibFuncExp() {
        double expected = 0.60653065971;
        std::string funcName = "exp";
        expFnTy fnPtr = (expFnTy) jit->lookupFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-7);
    }

    void CheckLibFuncFloor() {
        double expected = 3;
        std::string funcName = "floor";
        floorFnTy fnPtr = (floorFnTy) jit->lookupFunctionAddress(funcName);
        ASSERT_EQ(expected, fnPtr(3.7));
    }

    void CheckLibFuncLog() {
        double expected = 3.4657;
        std::string funcName = "log";
        logFnTy fnPtr = (logFnTy) jit->lookupFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(32), 1e-3);
    }

    void CheckLibFuncLog10() {
        double expected = 2;
        std::string funcName = "log10";
        log10FnTy fnPtr = (log10FnTy) jit->lookupFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(100), 1e-7);
    }

    void CheckLibFuncSin() {
        double expected = -0.4794255386;
        std::string funcName = "sin";
        sinFnTy fnPtr = (sinFnTy) jit->lookupFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-7);
    }

    void CheckLibFuncSinh() {
        double expected = -0.52109530549;
        std::string funcName = "sinh";
        sinhFnTy fnPtr = (sinhFnTy) jit->lookupFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-7);
    }

    void CheckLibFuncTan() {
        double expected = -0.54630248984;
        std::string funcName = "tan";
        tanFnTy fnPtr = (tanFnTy) jit->lookupFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-7);
    }

    void CheckLibFuncTanh() {
        double expected = -0.46211715726;
        std::string funcName = "tanh";
        tanhFnTy fnPtr = (tanhFnTy) jit->lookupFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-7);
    }

    void CheckLibFuncFmod() {
        double expected = 1;
        std::string funcName = "fmod";
        fmodFnTy fnPtr = (fmodFnTy) jit->lookupFunctionAddress(funcName);
        ASSERT_EQ(expected, fnPtr(5, 2));
    }

    void Check_arccotFnTy() {
        double expected = -1.1071487177940904;
        std::string funcName = "arccot";
        arccotFnTy fnPtr = (arccotFnTy) jit->lookupFunctionAddress(funcName);
        rrLogCriticalCiaran << fnPtr(-0.5);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-5);

    }

    void Check_rr_arccot_negzeroFnTy() {
        double expected = -1.1071487177940904;
        std::string funcName = "rr_arccot_negzero";
        rr_arccot_negzeroFnTy fnPtr = (rr_arccot_negzeroFnTy) jit->lookupFunctionAddress(
                funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-5);
    }

    void Check_arccothFnTy() {
        double expected = 0.423649;
        std::string funcName = "arccoth";
        arccothFnTy fnPtr = (arccothFnTy) jit->lookupFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(2.5), 1e-5);
    }

    void Check_arccscFnTy() {
        double expected = 0.41151684606748806;
        std::string funcName = "arccsc";
        arccscFnTy fnPtr = (arccscFnTy) jit->lookupFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(2.5), 1e-5);
    }

    void Check_arccschFnTy() {
        double expected = -1.4436354751788099;
        std::string funcName = "arccsch";
        arccschFnTy fnPtr = (arccschFnTy) jit->lookupFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-5);
    }

    void Check_arcsecFnTy() {
        double expected = 1.1592794807274085;
        std::string funcName = "arcsec";
        arcsecFnTy fnPtr = (arcsecFnTy) jit->lookupFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(2.5), 1e-5);
    }

    void Check_arcsechFnTy() {
        double expected = 1.31696;
        std::string funcName = "arcsech";
        arcsechFnTy fnPtr = (arcsechFnTy) jit->lookupFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(0.5), 1e-5);
    }

    void Check_cotFnTy() {
        double expected = -1.830487721712452;
        std::string funcName = "cot";
        cotFnTy fnPtr = (cotFnTy) jit->lookupFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-5);
    }

    void Check_cothFnTy() {
        double expected = -2.1639534137386525;
        std::string funcName = "coth";
        cothFnTy fnPtr = (cothFnTy) jit->lookupFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-5);
    }

    void Check_cscFnTy() {
        double expected = -2.0858296429334882;
        std::string funcName = "csc";
        cscFnTy fnPtr = (cscFnTy) jit->lookupFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-5);
    }

    void Check_cschFnTy() {
        double expected = 0.16528366985509557;
        std::string funcName = "csch";
        cschFnTy fnPtr = (cschFnTy) jit->lookupFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(2.5), 1e-5);
    }

    void Check_rr_factorialiFnTy() {
        double expected = 120;
        std::string funcName = "rr_factoriali";
        rr_factorialiFnTy fnPtr = (rr_factorialiFnTy) jit->lookupFunctionAddress(
                funcName);
        ASSERT_NEAR(expected, fnPtr(5), 1e-5);
    }

    void Check_rr_factorialdFnTy() {
        double expected = 24;
        std::string funcName = "rr_factoriald";
        rr_factorialdFnTy fnPtr = (rr_factorialdFnTy) jit->lookupFunctionAddress(
                funcName);
        ASSERT_NEAR(expected, fnPtr(4), 1e-5);
    }

    void Check_rr_logdFnTy() {
        double expected = 0.15051499783199057;
        std::string funcName = "rr_logd";
        rr_logdFnTy fnPtr = (rr_logdFnTy) jit->lookupFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(100, 2), 1e-5);
    }

    void Check_rr_rootdFnTy() {
        double expected = 2;
        std::string funcName = "rr_rootd";
        rr_rootdFnTy fnPtr = (rr_rootdFnTy) jit->lookupFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(2, 4), 1e-5);
    }

    void Check_secFnTy() {
        double expected = 1.139493927324549;
        std::string funcName = "sec";
        secFnTy fnPtr = (secFnTy) jit->lookupFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-5);
    }

    void Check_sechFnTy() {
        double expected = 0.88681888397007402;
        std::string funcName = "sech";
        sechFnTy fnPtr = (sechFnTy) jit->lookupFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-5);
    }

    void Check_arccoshFnTy() {
        double expected = 1.5667992369724109;
        std::string funcName = "arccosh";
        arccoshFnTy fnPtr = (arccoshFnTy) jit->lookupFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(2.5), 1e-5);
    }

    void Check_arcsinhFnTy() {
        double expected = -0.48121182505960347;
        std::string funcName = "arcsinh";
        arcsinhFnTy fnPtr = (arcsinhFnTy) jit->lookupFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-5);
    }

    void Check_arctanhFnTy() {
        double expected = -0.54930614433405489;
        std::string funcName = "arctanh";
        arctanhFnTy fnPtr = (arctanhFnTy) jit->lookupFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(-0.5), 1e-5);
    }

    void Check_quotientFnTy() {
        double expected = 5;
        std::string funcName = "quotient";
        quotientFnTy fnPtr = (quotientFnTy) jit->lookupFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(10, 2), 1e-5);
    }

    void Check_rr_maxFnTy() {
        double expected = 4;
        std::string funcName = "rr_max";
        rr_maxFnTy fnPtr = (rr_maxFnTy) jit->lookupFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(3, 4), 1e-5);
    }

    void Check_rr_minFnTy() {
        double expected = 3;
        std::string funcName = "rr_min";
        rr_minFnTy fnPtr = (rr_minFnTy) jit->lookupFunctionAddress(funcName);
        ASSERT_NEAR(expected, fnPtr(3, 4), 1e-5);
    }

    void Check_rr_csr_matrix_get_nz() {
        double expected = 3;
        std::string funcName = "csr_matrix_get_nz";
        csr_matrix_get_nz_FnTy csr_matrix_get_nz = (csr_matrix_get_nz_FnTy) jit->lookupFunctionAddress(funcName);
        ASSERT_FALSE(csr_matrix_get_nz == nullptr);
    }


    void Check_rr_csr_matrix_set_nz() {
        std::string funcName = "csr_matrix_set_nz";
        csr_matrix_set_nz_FnTy csr_matrix_set_nz = (csr_matrix_set_nz_FnTy) jit->lookupFunctionAddress(funcName);
        std::cout << csr_matrix_set_nz << std::endl;
        ASSERT_FALSE(csr_matrix_set_nz == nullptr);
    }


    void check_pow_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "pow");
    }

    void check_fabs_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "fabs");
    }

    void check_acos_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "acos");
    }

    void check_asin_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "asin");
    }

    void check_atan_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "atan");
    }

    void check_ceil_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "ceil");
    }

    void check_cos_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "cos");
    }

    void check_cosh_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "cosh");
    }

    void check_exp_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "exp");
    }

    void check_floor_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "floor");
    }

    void check_log_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "log");
    }

    void check_log10_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "log10");
    }

    void check_sin_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "sin");
    }

    void check_sinh_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "sinh");
    }

    void check_tan_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "tan");
    }

    void check_tanh_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "tanh");
    }

    void check_fmod_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "fmod");
    }

    void check_arccot_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "arccot");
    }

    void check_rr_arccot_negzero_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "rr_arccot_negzero");
    }

    void check_arccoth_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "arccoth");
    }

    void check_arccsc_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "arccsc");
    }

    void check_arccsch_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "arccsch");
    }

    void check_arcsec_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "arcsec");
    }

    void check_arcsech_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "arcsech");
    }

    void check_cot_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "cot");
    }

    void check_coth_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "coth");
    }

    void check_csc_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "csc");
    }

    void check_csch_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "csch");
    }

    void check_rr_factoriali_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "rr_factoriali");
    }

    void check_rr_factoriald_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "rr_factoriald");
    }

    void check_rr_logd_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "rr_logd");
    }

    void check_rr_rootd_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "rr_rootd");
    }

    void check_sec_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "sec");
    }

    void check_sech_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "sech");
    }

    void check_arccosh_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "arccosh");
    }

    void check_arcsinh_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "arcsinh");
    }

    void check_arctanh_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "arctanh");
    }

    void check_quotient_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "quotient");
    }

    void check_rr_max_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "rr_max");
    }

    void check_rr_min_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "rr_min");
    }

    void check_csr_matrix_get_nz_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "csr_matrix_get_nz");
    }

    void check_csr_matrix_set_nz_ViaModule(){
        checkFunctionLoadsUsingModule(jit.get(), "csr_matrix_set_nz");
    }

#ifdef LIBSBML_HAS_PACKAGE_DISTRIB
    void check_rr_distrib_uniform_LoadsFromModule(){
        checkFunctionLoadsUsingModule(jit.get(), "rr_distrib_uniform");
    }
    void check_rr_distrib_normal_LoadsFromModule(){
        checkFunctionLoadsUsingModule(jit.get(), "rr_distrib_normal");
    }
    void check_rr_distrib_normal_four_LoadsFromModule(){
        checkFunctionLoadsUsingModule(jit.get(), "rr_distrib_normal_four");
    }
    void check_rr_distrib_bernoulli_LoadsFromModule(){
        checkFunctionLoadsUsingModule(jit.get(), "rr_distrib_bernoulli");
    }
    void check_rr_distrib_binomial_LoadsFromModule(){
        checkFunctionLoadsUsingModule(jit.get(), "rr_distrib_binomial");
    }
    void check_rr_distrib_binomial_four_LoadsFromModule(){
        checkFunctionLoadsUsingModule(jit.get(), "rr_distrib_binomial_four");
    }
    void check_rr_distrib_cauchy_LoadsFromModule(){
        checkFunctionLoadsUsingModule(jit.get(), "rr_distrib_cauchy");
    }
    void check_rr_distrib_cauchy_one_LoadsFromModule(){
        checkFunctionLoadsUsingModule(jit.get(), "rr_distrib_cauchy_one");
    }
    void check_rr_distrib_cauchy_four_LoadsFromModule(){
        checkFunctionLoadsUsingModule(jit.get(), "rr_distrib_cauchy_four");
    }
    void check_rr_distrib_chisquare_LoadsFromModule(){
        checkFunctionLoadsUsingModule(jit.get(), "rr_distrib_chisquare");
    }
    void check_rr_distrib_chisquare_three_LoadsFromModule(){
        checkFunctionLoadsUsingModule(jit.get(), "rr_distrib_chisquare_three");
    }
    void check_rr_distrib_exponential_LoadsFromModule(){
        checkFunctionLoadsUsingModule(jit.get(), "rr_distrib_exponential");
    }
    void check_rr_distrib_exponential_three_LoadsFromModule(){
        checkFunctionLoadsUsingModule(jit.get(), "rr_distrib_exponential_three");
    }
    void check_rr_distrib_gamma_LoadsFromModule(){
        checkFunctionLoadsUsingModule(jit.get(), "rr_distrib_gamma");
    }
    void check_rr_distrib_gamma_four_LoadsFromModule(){
        checkFunctionLoadsUsingModule(jit.get(), "rr_distrib_gamma_four");
    }
    void check_rr_distrib_laplace_LoadsFromModule(){
        checkFunctionLoadsUsingModule(jit.get(), "rr_distrib_laplace");
    }
    void check_rr_distrib_laplace_one_LoadsFromModule(){
        checkFunctionLoadsUsingModule(jit.get(), "rr_distrib_laplace_one");
    }
    void check_rr_distrib_laplace_four_LoadsFromModule(){
        checkFunctionLoadsUsingModule(jit.get(), "rr_distrib_laplace_four");
    }
    void check_rr_distrib_lognormal_LoadsFromModule(){
        checkFunctionLoadsUsingModule(jit.get(), "rr_distrib_lognormal");
    }
    void check_rr_distrib_lognormal_four_LoadsFromModule(){
        checkFunctionLoadsUsingModule(jit.get(), "rr_distrib_lognormal_four");
    }
    void check_rr_distrib_poisson_LoadsFromModule(){
        checkFunctionLoadsUsingModule(jit.get(), "rr_distrib_poisson");
    }
    void check_rr_distrib_poisson_three_LoadsFromModule(){
        checkFunctionLoadsUsingModule(jit.get(), "rr_distrib_poisson_three");
    }
    void check_rr_distrib_rayleigh_LoadsFromModule(){
        checkFunctionLoadsUsingModule(jit.get(), "rr_distrib_rayleigh");
    }
    void check_rr_distrib_rayleigh_three_LoadsFromModule(){
        checkFunctionLoadsUsingModule(jit.get(), "rr_distrib_rayleigh_three");
    }
#endif


protected:

    // non owning
    std::unique_ptr<Jit> jit;
    LoadSBMLOptions opt;

};


#endif //ROADRUNNER_MAPFUNCTIONSTOJITSYMBOLSTESTS_H
