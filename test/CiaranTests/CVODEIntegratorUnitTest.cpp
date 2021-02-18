

#include "gtest/gtest.h"
#include "gmock/gmock.h"

// code under test
#include "CVODEIntegrator.h"

// mockup of executable model interfaec
#include "MockExecutableModel.h"

using namespace rr;
using namespace testing;
/**
 * Warning: Rant.
 *
 * CVODEIntegrator static vs public methods
 * -----------------------------------------
 *
 * I don't know what the objective was but having a static method
 * which then gets used by a pubilc method for returning information
 * is just bad design - because now we have two methods which do the same
 * thing. For instance :
 *
 *     std::string CVODEIntegrator::getHint() const {
 *          return CVODEIntegrator::getCVODEIntegratorHint();
 *      }
 *
 *      std::string CVODEIntegrator::getCVODEIntegratorHint() {
 *          return "Deterministic ODE solver";
 *      }
 * Either just have a static method and use it or just have a public method
 * return a non-statically allocated string each time.
 *
 * The ExecutableModel pure abstract virtual class (aka an interface).
 * ---------------------------------------------------------------------
 * - This is a bloated class that seriously violates the
 *   inteface segregation rule.
 *
 * standard namespace
 * ------------------
 * Why do we use "using namespace std"? This is coding 101.
 *
 *
 * Rant over.
 *
 */

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

TEST_F(CVODEIntegratorTests, CreateCVODE) {
    // When we create a CVODEIntegrator the createCVODE method is called
    CVODEIntegrator integrator(&mockExecutableModel);

    // check that mStateVector, a sundials NVector is initialized.
//    integrator


}

// other methods to test:
//setConcentrationTolerance
//updateCVODE
//integrate
//createCVode
































