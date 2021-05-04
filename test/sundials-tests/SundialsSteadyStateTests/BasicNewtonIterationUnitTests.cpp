#include <fstream>
#include <rr-libstruct/lsLibStructural.h>
#include "gtest/gtest.h"

#include "SundialsSteadyStateSolverUnitTest.h"
#include "BasicNewtonIteration.h"
#include "rrRoadRunner.h"
#include "TestModelFactory.h"

using namespace rr;

/**
 * @brief tests that focus on the "newton" solver from sundials test suite
 * without linesearch globalisation
 */
class BasicNewtonIterationUnitTest : public SundialsSteadyStateSolverUnitTest {
public:

    BasicNewtonIterationUnitTest() : SundialsSteadyStateSolverUnitTest() {};

    ~BasicNewtonIterationUnitTest() override = default;
};


TEST_F(BasicNewtonIterationUnitTest, SolveUsingSolverDirectly) {
    // aka without a RoadRunner instance
    BasicNewtonIteration solver(rr->getModel());
    solver.solve();
    checkResults(rr->getFloatingSpeciesConcentrationsNamedArray());
}

TEST_F(BasicNewtonIterationUnitTest, ChangeAndResetSettings) {
    BasicNewtonIteration solver(rr->getModel());
    solver.setValue("eta_param_gamma", Setting(0.4356));
    solver.resetSettings();
    ASSERT_EQ((int)solver.getValue("eta_param_gamma"), 0);
}


TEST_F(BasicNewtonIterationUnitTest, RegenerateTheModelBeforeCreatingSolver) {
    rr->regenerateModel();
    BasicNewtonIteration solver(rr->getModel());
    solver.solve();
    checkResults(rr->getFloatingSpeciesConcentrationsNamedArray());
}

TEST_F(BasicNewtonIterationUnitTest, RegenerateTheModelAfterCreatingSolver) {
    BasicNewtonIteration solver(rr->getModel());
    rr->regenerateModel();
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
        public BasicNewtonIterationUnitTest,
        public ::testing::WithParamInterface<std::pair<std::string, Setting>> {
public:
    SettingsTests() = default;
};

TEST_P(SettingsTests, TestSettings) {
    // pair of string: variant
    auto settingUnderTest = GetParam();

    // unpack the test parameters
    std::string settingName = settingUnderTest.first;
    Setting settingValue = settingUnderTest.second;
    std::cout << "Testing setting \"" << settingName << "\"" << std::endl;

    // a bit of a faffy way to downcast
    OpenLinearFlux testModel;

    RoadRunner rr(testModel.str());
    BasicNewtonIteration solver(rr.getModel());

    // actually change the setting to our new value
    solver.setValue(settingName, settingValue);

//    // make comparison between setting value and value we get from solver
//    if (settingValue.isInteger()) {
//        EXPECT_EQ((int) settingValue, (int)solver.getValue(settingName));
//    } else if (settingValue.isNumeric()) {
//        EXPECT_NEAR((double) settingValue, solver.getValue(settingName), 0.001);
//    } else if (settingValue.isBool()) {
//        EXPECT_TRUE((bool) settingValue == (bool)solver.getValue(settingName));
//    } else if (settingValue.isString()) {
//        EXPECT_STREQ(((std::string) settingValue).c_str(), ((std::string)solver.getValue(settingName)).c_str());
//    }
}

/**
 * Run the parameterized test suite "SettingsTests" with these parameter
 * combinations. The first item of the tuple is the parameter name, the
 * second is its value, stored as a rr::Setting. It is sometimes useful
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
//                std::pair<std::string, Setting>("num_max_iters", 153),
//                std::pair<std::string, Setting>("print_level", 2),
//                std::pair<std::string, Setting>("no_init_setup", true),
                std::pair<std::string, Setting>("eta_form", Setting(std::string("eta_choice2")))
//                std::pair<std::string, Setting>("no_res_monitoring", true),
//                std::pair<std::string, Setting>("max_setup_calls", 974),
//                std::pair<std::string, Setting>("max_subsetup_calls", 132),
//                std::pair<std::string, Setting>("eta_constant_value", 6),
//                std::pair<std::string, Setting>("eta_param_gamma", 0.5234),
//                std::pair<std::string, Setting>("eta_param_alpha", 0.1234),
//                std::pair<std::string, Setting>("res_mon_min", 0.01),
//                std::pair<std::string, Setting>("res_mon_max", 0.9999),
//                std::pair<std::string, Setting>("res_mon_constant_value", 0.25),
//                std::pair<std::string, Setting>("no_min_eps", true),
//                std::pair<std::string, Setting>("max_newton_step", 15),
//                std::pair<std::string, Setting>("max_beta_fails", 3),
//                std::pair<std::string, Setting>("func_norm_tol", 0.5),
//                std::pair<std::string, Setting>("scaled_step_tol", 0.3)

//                std::pair<std::string, Setting>("maa", 4),
//                std::pair<std::string, Setting>("damping_aa", 0.35)
        )
);





















