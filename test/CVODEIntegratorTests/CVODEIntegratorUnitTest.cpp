

#include "gtest/gtest.h"
#include "gmock/gmock.h"

// code under test
#include "CVODEIntegrator.h"

// mockup of executable model interfaec
#include "MockExecutableModel.h"

using namespace rr;
using namespace testing;

/**
 * Test fixture for CVODEIntegrator class
 */
class CVODEIntegratorTests : public ::testing::Test {
public:

    MockExecutableModel mockExecutableModel;

    CVODEIntegratorTests() {
        Logger::setLevel(Logger::LOG_DEBUG);
    };

};

TEST_F(CVODEIntegratorTests, getName) {
    CVODEIntegrator integrator(&mockExecutableModel);
    const std::string &actual = integrator.getName();
    std::cout << actual << std::endl;
    ASSERT_STREQ("cvode", actual.c_str());
}

TEST_F(CVODEIntegratorTests, getCVODEIntegratorName) {
    const std::string &actual = CVODEIntegrator::getCVODEIntegratorName();
    std::cout << actual << std::endl;
    ASSERT_STREQ("cvode", actual.c_str());
}

TEST_F(CVODEIntegratorTests, getDescription) {
    CVODEIntegrator integrator(&mockExecutableModel);
    const std::string &actual = integrator.getDescription();
    std::cout << actual << std::endl;
    std::string expected = "CVODE is a deterministic ODE solver from the SUNDIALS suite of timecourse integrators. It implements an Adams-Moulton solver for non-stiff problems and a backward differentiation formula (BDF) solver for stiff problems.";
    ASSERT_STREQ(expected.c_str(), actual.c_str());
}

TEST_F(CVODEIntegratorTests, getHint) {
    CVODEIntegrator integrator(&mockExecutableModel);
    const std::string &actual = integrator.getHint();
    std::cout << actual << std::endl;
}


TEST_F(CVODEIntegratorTests, getIntegrationMethod) {
    CVODEIntegrator integrator(&mockExecutableModel);
    Integrator::IntegrationMethod actual = integrator.getIntegrationMethod();
    std::cout << actual << std::endl;
    ASSERT_EQ(Integrator::IntegrationMethod::Deterministic, actual);
}


TEST_F(CVODEIntegratorTests, CheckThatWeCanChangeIndvidualTolerances) {
    // when called with argument "0", the mock getFloatingSpeciesIndex
    //  method will return the integer 0
    EXPECT_CALL(mockExecutableModel, getFloatingSpeciesIndex("A"))
            .WillRepeatedly(Return(0));

    EXPECT_CALL(mockExecutableModel, getNumIndFloatingSpecies)
            .WillRepeatedly(Return(0));

    EXPECT_CALL(mockExecutableModel, getNumFloatingSpecies)
            .WillRepeatedly(Return(3));

    EXPECT_CALL(mockExecutableModel, getNumRateRules)
            .WillRepeatedly(Return(3));

    std::vector<std::string> r({"A", "B", "C"});
    EXPECT_CALL(mockExecutableModel, getRateRuleSymbols)
            .WillRepeatedly(Return(r));

    CVODEIntegrator integrator(&mockExecutableModel);
    integrator.setIndividualTolerance("A", 0.000001);

    Variant atol = integrator.getValue("absolute_tolerance");
    ASSERT_TRUE(atol.isDoubleVector());
    std::vector<double> actual = atol.convert<std::vector<double>>();
    ASSERT_EQ(actual[0], 0.000001);
}

TEST_F(CVODEIntegratorTests, CheckWeCanTurnStiffToFalse) {
    CVODEIntegrator integrator(&mockExecutableModel);
    integrator.setValue("stiff", false);
    ASSERT_FALSE(integrator.getValue("stiff"));
}

TEST_F(CVODEIntegratorTests, CheckWeCanTurnStiffToTrue) {
    CVODEIntegrator integrator(&mockExecutableModel);
    integrator.setValue("stiff", true);
    ASSERT_TRUE(integrator.getValue("stiff"));
}

TEST_F(CVODEIntegratorTests, CheckIntegrateReturnsCorrectSimulationEndTime) {
    CVODEIntegrator integrator(&mockExecutableModel);
    double timeEnd = integrator.integrate(0.0, 0.01);
    double expected = 0.01;
    ASSERT_EQ(expected, timeEnd);
}

/**
 * If a model has no state variables, the CvodeCreate does not crate
 * an integrator and it stays a nullptr, as initialized in constructor
 *
 */
TEST_F(CVODEIntegratorTests, CheckConstructorDoesNotCreateSundialsObjectsWhenModelHas0StateVariables) {
    EXPECT_CALL(mockExecutableModel, getStateVector(nullptr))
            .WillRepeatedly(Return(0));
    CVODEIntegrator integrator(&mockExecutableModel);
    ASSERT_EQ(integrator.getStateVector(), nullptr);
    ASSERT_EQ(integrator.getCvodeMemory(), nullptr);
    ASSERT_EQ(integrator.getLinearSolver(), nullptr);
    ASSERT_EQ(integrator.getJac(), nullptr);
}

/**
 * When a model has state variables, we expact an integrator to be created
 */
TEST_F(CVODEIntegratorTests, CheckConstructorInitSundialsObjectsWhenGT0StateVariables) {
    EXPECT_CALL(mockExecutableModel, getStateVector(nullptr))
            .WillRepeatedly(Return(3));
    CVODEIntegrator integrator(&mockExecutableModel);
    ASSERT_NE(integrator.getStateVector(), nullptr);
    ASSERT_NE(integrator.getCvodeMemory(), nullptr);
    ASSERT_NE(integrator.getLinearSolver(), nullptr);
    ASSERT_NE(integrator.getJac(), nullptr);
}

/**
 * Once an integrator is created, we need to be able to destroy it again
 */
TEST_F(CVODEIntegratorTests, CheckThatWeCanDestroyIntegrator) {
    EXPECT_CALL(mockExecutableModel, getStateVector(nullptr))
            .WillRepeatedly(Return(3));
    CVODEIntegrator integrator(&mockExecutableModel);
    integrator.freeCVode();
    ASSERT_EQ(integrator.getStateVector(), nullptr);
    ASSERT_EQ(integrator.getCvodeMemory(), nullptr);
    ASSERT_EQ(integrator.getLinearSolver(), nullptr);
    ASSERT_EQ(integrator.getJac(), nullptr);
}

/**
 * Once an integrator is created and distroyed, we want to be able
 * to create it again.
 */
TEST_F(CVODEIntegratorTests, CheckThatWeCanRecreateCvodeAfterDestroy) {
    EXPECT_CALL(mockExecutableModel, getStateVector(nullptr))
            .WillRepeatedly(Return(3));
    CVODEIntegrator integrator(&mockExecutableModel);
    integrator.freeCVode();
    integrator.createCVode();
    ASSERT_NE(integrator.getStateVector(), nullptr);
    ASSERT_NE(integrator.getCvodeMemory(), nullptr);
    ASSERT_NE(integrator.getLinearSolver(), nullptr);
    ASSERT_NE(integrator.getJac(), nullptr);
}

/**
 * We compute next time step with stiff solver, then with
 * non-stiff solver and make sure they are doing different
 * things.
 */
TEST_F(CVODEIntegratorTests, CheckStiffVsNonStiff) {
    // we need to tell our mock to return 3 when CVODEIntegrator
    // ask how many state variables the model has, or sundials objects
    // will not be created
    EXPECT_CALL(mockExecutableModel, getStateVector(nullptr))
            .WillRepeatedly(Return(3));

    CVODEIntegrator integrator(&mockExecutableModel);
    // integrate 0.1 time steps
    integrator.integrate(0.0, 0.1);

    // result from integrator with default "stiff" setting
    auto result1 = integrator.getStateVector();

    // restart the clock
    integrator.restart(0.0);

    // change to non-stiff solver
//    integrat

}


// other methods to test:
//setConcentrationTolerance
//updateCVODE
//integrate
//createCVode
//loadConfigSettings
//loadSBMLSettings
//setValue
//resetSettings
//integrate
//restart































