#include <fstream>
#include "gtest/gtest.h"

#include "NewtonIteration.h"
#include "rrRoadRunner.h"
#include "SBMLTestModelFactory.h"

using namespace rr;

/**
 * Takes test model name as argument
 */
class NewtonIterationUnitTests : public ::testing::Test {

public:
    RoadRunner rr;

    // we introduce this test dependency (in the absence of mocking).
    // These unit tests are tightly coupled to this model.
    OpenLinearFlux *testModel = (OpenLinearFlux *) SBMLTestModelFactory("OpenLinearFlux");

    NewtonIterationUnitTests() {
        rr.load(testModel->str()); // load our sbml
    };

    void checkResults(ls::DoubleMatrix result) const {
        std::vector<std::string> names = result.getColNames();
        for (int i = 0; i < names.size(); i++) {
            std::string speciesID = names[i];
            double actualResult = result[0][i]; // 0th row, ith col of a DoubleMatrix
            double expected = testModel->steadyState()[speciesID].second; // first is start val, second is speciesID at steady state
            std::cout << "Comparing \"" << speciesID << "\" expected result: " << expected
                      << " with actual result " << actualResult << std::endl;
            EXPECT_NEAR(expected, actualResult, 0.0001);
        }
    }
};

/**
 *
 */
TEST_F(NewtonIterationUnitTests, solve) {
    NewtonIteration solver(rr.getModel());
    solver.solve();
    checkResults(rr.getFloatingSpeciesConcentrationsNamedArray());
}

TEST_F(NewtonIterationUnitTests, CheckWeCanChangeAndResetSettings) {
    NewtonIteration solver(rr.getModel());
    solver.setValue("strategy", Variant("linesearch"));
    solver.resetSettings();
    ASSERT_STREQ("basic", solver.getValueAsString("strategy").c_str());
}


TEST_F(NewtonIterationUnitTests, CheckWeCanRegenerateTheModelBeforeCreatingSolver) {
    rr.regenerate();
    NewtonIteration solver(rr.getModel());
    solver.solve();
    checkResults(rr.getFloatingSpeciesConcentrationsNamedArray());
}

TEST_F(NewtonIterationUnitTests, CheckWeCanRegenerateTheModelAfterCreatingSolver) {
    NewtonIteration solver(rr.getModel());
    rr.regenerate();
    // after regeneration, the pointer to the model is different
    // so we must sync with model before solving.
    solver.syncWithModel(rr.getModel());
    solver.solve();
    checkResults(rr.getFloatingSpeciesConcentrationsNamedArray());
}

TEST_F(NewtonIterationUnitTests, TestUpdate) {
    NewtonIteration solver(rr.getModel());
}

/**
 * Tests each of the settings can be changed
 */
class SettingsTests :
        public NewtonIterationUnitTests,
        public ::testing::WithParamInterface<std::pair<std::string, Variant>> {
public:
    SettingsTests() = default;
};

TEST_P(SettingsTests, TestSettings) {
    // pair of string: variant
    auto settingUnderTest = GetParam();

    // unpack the test parameters
    std::string settingName = settingUnderTest.first;
    Variant settingValue = settingUnderTest.second;
    std::cout << "Testing setting \"" << settingName << "\"" << std::endl;

    auto *testModel = (OpenLinearFlux *) SBMLTestModelFactory("OpenLinearFlux");
    RoadRunner rr(testModel->str());
    NewtonIteration solver(rr.getModel());

    // actually change the setting to our new value
    solver.setValue(settingName, settingValue);

    // make comparison between setting value and value we get from solver
    if (settingValue.isInteger()) {
        EXPECT_EQ((int) settingValue, solver.getValueAsInt(settingName));
    } else if (settingValue.isNumeric()) {
        EXPECT_NEAR((double) settingValue, solver.getValueAsDouble(settingName), 0.001);
    } else if (settingValue.isBool()) {
        EXPECT_TRUE((bool) settingValue == solver.getValueAsBool(settingName));
    } else if (settingValue.isString()) {
        EXPECT_STREQ(((std::string) settingValue).c_str(), solver.getValueAsString(settingName).c_str());
    }
}

/**
 * Run the parameterized test suite "SettingsTests" with these parameter
 * combinations. The first item of the tuple is the parameter name, the
 * second is its value, stored as a rr::Variant. It is sometimes useful
 * to run these one at a time. To do so, just temporarily comment out
 * the ones you dont want to run.
 *
 * Note this does not test the interface between roadrunner and kinsol,
 * only the users ability to change a parameter in roadrunner.
 */
INSTANTIATE_TEST_SUITE_P(
        SettingsTests,
        SettingsTests,
        ::testing::Values(
                std::pair<std::string, Variant>("strategy", Variant("linesearch")),

                std::pair<std::string, Variant>("NumMaxIters", Variant(153)),
                std::pair<std::string, Variant>("PrintLevel", Variant(2)),
                std::pair<std::string, Variant>("EtaForm", Variant("eta_choice2")),
                std::pair<std::string, Variant>("NoInitSetup", Variant(true)),
                std::pair<std::string, Variant>("NoResMon", Variant(true)),
                std::pair<std::string, Variant>("MaxSetupCalls", Variant(974)),
                std::pair<std::string, Variant>("MaxSubSetupCalls", Variant(132)),
                std::pair<std::string, Variant>("EtaConstValue", Variant(6)),
                std::pair<std::string, Variant>("EtaParamGamma", Variant(0.5234)),
                std::pair<std::string, Variant>("EtaParamAlpha", Variant(0.1234)),
                std::pair<std::string, Variant>("ResMonMin", Variant(0.01)),
                std::pair<std::string, Variant>("ResMonMax", Variant(0.9999)),
                std::pair<std::string, Variant>("ResMonConstValue", Variant(0.25)),
                std::pair<std::string, Variant>("NoMinEps", Variant(true)),
                std::pair<std::string, Variant>("MaxNewtonStep", Variant(15)),
                std::pair<std::string, Variant>("MaxBetaFails", Variant(3)),
                std::pair<std::string, Variant>("FuncNormTol", Variant(0.5)),
                std::pair<std::string, Variant>("ScaledSteptol", Variant(0.3)),
                std::pair<std::string, Variant>("MAA", Variant(4)),
                std::pair<std::string, Variant>("DampingAA", Variant(0.35))
        )
);



/**
 * todo rr has an akward set up right now regarding registration of solvers.
 *  problems
 *  1) need both static and non-static implementation of same method in solvers.
 *  2) solver registration class should be renamed steady state solver reg
 *  3) I thought the point of registrar was so that solvers are not instantiated until needed.
 *      But digging deeper, I think they are all allocated with every rr model anyway.
 *  4) Solver registration manager has static method for making it a singleton.
 *      - Note, we cannot make solvers singletons as they will break
 *        if we try to solve multiple models at the same time with it.
 *  5) Why is roadrunner managing memory for solvers? Why not use
 *      RAII for what is was designed for?
 */


















