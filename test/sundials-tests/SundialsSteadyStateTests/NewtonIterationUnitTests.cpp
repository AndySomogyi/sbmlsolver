#include <fstream>
#include <rr-libstruct/lsLibStructural.h>
#include "gtest/gtest.h"

#include "SundialsSteadyStateSolverUnitTest.h"
#include "NewtonIteration.h"
#include "rrRoadRunner.h"
#include "TestModelFactory.h"

using namespace rr;

/**
 * Takes test model name as argument
 */
class NewtonIterationUnitTests : public SundialsSteadyStateSolverUnitTest {
public:

    NewtonIterationUnitTests(): SundialsSteadyStateSolverUnitTest() {};

    ~NewtonIterationUnitTests() override = default;
};


TEST_F(NewtonIterationUnitTests, SolveUsingSolverDirectly) {
    // aka without a RoadRunner instance
    NewtonIteration solver(rr->getModel());
    solver.solve();
    checkResults(rr->getFloatingSpeciesConcentrationsNamedArray());
}

TEST_F(NewtonIterationUnitTests, ChangeAndResetSettings) {
    NewtonIteration solver(rr->getModel());
    solver.setValue("strategy", Variant("linesearch"));
    solver.resetSettings();
    ASSERT_STREQ("basic", solver.getValueAsString("strategy").c_str());
}


TEST_F(NewtonIterationUnitTests, RegenerateTheModelBeforeCreatingSolver) {
    rr->regenerate();
    NewtonIteration solver(rr->getModel());
    solver.solve();
    checkResults(rr->getFloatingSpeciesConcentrationsNamedArray());
}

TEST_F(NewtonIterationUnitTests, RegenerateTheModelAfterCreatingSolver) {
    NewtonIteration solver(rr->getModel());
    rr->regenerate();
    // after regeneration, the pointer to the model is different
    // so we must sync with model before solving.
    solver.syncWithModel(rr->getModel());
    solver.solve();
    checkResults(rr->getFloatingSpeciesConcentrationsNamedArray());
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

    // a bit of a faffy way to downcast
    OpenLinearFlux testModel;

    RoadRunner rr(testModel.str());
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





















