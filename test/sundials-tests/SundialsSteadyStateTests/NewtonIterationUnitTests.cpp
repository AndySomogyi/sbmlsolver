#include <fstream>
#include <rr-libstruct/lsLibStructural.h>
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
    std::unique_ptr<SBMLTestModel> testModelPtr = SBMLTestModelFactory("OpenLinearFlux");
    std::unique_ptr<OpenLinearFlux> testModel;


    NewtonIterationUnitTests() {
        testModel = std::unique_ptr<OpenLinearFlux>(dynamic_cast<OpenLinearFlux*>(testModelPtr.release()));
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

//TEST_F(NewtonIterationUnitTests, TestPresimulation) {
//    ls::DoubleMatrix before = rr.getFloatingSpeciesConcentrationsNamedArray();
//    Presimulation presimulation(rr.getModel(), 10, 1000, false);
//    presimulation.simulate();
//    ls::DoubleMatrix after = rr.getFloatingSpeciesConcentrationsNamedArray();
//    // it is the domain of CVODEIntegrator tests to ensure model
//    // is integrated correctly. Here, we only care that state vector
//    // before and after are different.
//
//    // No equality operators in ls::DoubleMatrix so we need to do it ourselves. Sad face =[
//    for (int i = 0; i < before.numRows(); i++) {
//        for (int j = 0; j < before.numCols(); j++) {
//            std::cout << "comparing before: " << before(i, j) << " to after: " << after(i, j) << std::endl;
//            ASSERT_NE(before(i, j), after(i, j));
//        }
//    }
//}

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

    // a bit of a faffy way to downcast
    std::unique_ptr<SBMLTestModel> testModelPtr = SBMLTestModelFactory("OpenLinearFlux");
    auto* testModelPtrRaw = dynamic_cast<OpenLinearFlux*>(testModelPtr.get());
    std::unique_ptr<OpenLinearFlux> testModel;
    testModelPtr.release();
    testModel.reset(testModelPtrRaw);

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
                std::pair<std::string, Variant>("num_max_iters", Variant(153)),
                std::pair<std::string, Variant>("print_level", Variant(2)),
                std::pair<std::string, Variant>("eta_form", Variant("eta_choice2")),
                std::pair<std::string, Variant>("no_init_setup", Variant(true)),
                std::pair<std::string, Variant>("no_res_monitoring", Variant(true)),
                std::pair<std::string, Variant>("max_setup_calls", Variant(974)),
                std::pair<std::string, Variant>("max_subsetup_calls", Variant(132)),
                std::pair<std::string, Variant>("eta_constant_value", Variant(6)),
                std::pair<std::string, Variant>("eta_param_gamma", Variant(0.5234)),
                std::pair<std::string, Variant>("eta_param_alpha", Variant(0.1234)),
                std::pair<std::string, Variant>("res_mon_min", Variant(0.01)),
                std::pair<std::string, Variant>("res_mon_max", Variant(0.9999)),
                std::pair<std::string, Variant>("res_mon_constant_value", Variant(0.25)),
                std::pair<std::string, Variant>("no_min_eps", Variant(true)),
                std::pair<std::string, Variant>("max_newton_step", Variant(15)),
                std::pair<std::string, Variant>("max_beta_fails", Variant(3)),
                std::pair<std::string, Variant>("func_norm_tol", Variant(0.5)),
                std::pair<std::string, Variant>("scaled_step_tol", Variant(0.3)),
                std::pair<std::string, Variant>("maa", Variant(4)),
                std::pair<std::string, Variant>("damping_aa", Variant(0.35))
        )
);





















