

#include <SBMLTestModelFactory.h>
#include "gtest/gtest.h"

#include "KinsolSteadyStateSolver.h"

using namespace rr;

/**
 * The KinsolSteadyStateSolver class is the superclass of
 * all kinsol solvers. It has abstract methods which are
 * not under test in this unit. We define this fake class
 * which minimally implements the abstract methods.
 */
class DerivedKinsolSteadyStateSolver : public KinsolSteadyStateSolver {
public:
    using KinsolSteadyStateSolver::KinsolSteadyStateSolver;

    ~DerivedKinsolSteadyStateSolver() override = default;

    DerivedKinsolSteadyStateSolver() = default;

    void syncWithModel(ExecutableModel *m) override {};

    void freeKinsol() override {};

    void updateKinsol() override {};

    void resetSettings() override {};

    std::string getName() const override {
        return "DerivedSteadyStateSolver";
    }

    std::string getDescription() const override {
        return "A fake solver used for testing";
    }

    std::string getHint() const override {
        return "A fake solver used for testing";
    }

    double solve() override {
        return 1.0;
    };

    void createKinsol() override {};


};


class KinsolSteadyStateSolverTests : public ::testing::Test {
public:
    KinsolSteadyStateSolverTests() = default;
};


TEST_F(KinsolSteadyStateSolverTests, CheckTheKinsolDyDtFcnReturnsCorrectValues) {
    DerivedKinsolSteadyStateSolver solver;
}

TEST_F(KinsolSteadyStateSolverTests, CheckSettingUScaleParameterFromScalar) {

}

TEST_F(KinsolSteadyStateSolverTests, CheckSettingFScaleParameterFromScalar) {

}

TEST_F(KinsolSteadyStateSolverTests, CheckSettingUScaleParameterFromVector) {

}

TEST_F(KinsolSteadyStateSolverTests, CheckSettingFScaleParameterFromVector) {

}

TEST_F(KinsolSteadyStateSolverTests, CheckCreateKinsolCreatesTheRightObjects) {

}

TEST_F(KinsolSteadyStateSolverTests, CheckFreeKinsolFreesTheRightObjects) {

}


class SettingsTests :
        public KinsolSteadyStateSolverTests,
        public ::testing::WithParamInterface<std::pair<std::string, Variant>> {
};

TEST_P(SettingsTests, TestSettings) {
    // pair of string: variant
    auto settingUnderTest = GetParam();
    std::string settingName = settingUnderTest.first;
    Variant settingValue = settingUnderTest.second;

    DerivedKinsolSteadyStateSolver solver;

    if (settingValue.isInteger()) {
        ASSERT_EQ((int) settingValue, solver.getValueAsInt(settingName));
    } else if (settingValue.isNumeric()) {
        ASSERT_NEAR((double) settingValue, solver.getValueAsDouble(settingName), 0.001);
    } else if (settingValue.isBool()) {
        ASSERT_TRUE((bool) settingValue == solver.getValueAsBool(settingName));
    } else if (settingValue.isString()) {
        ASSERT_STREQ(((std::string) settingValue).c_str(), solver.getValueAsString(settingName).c_str());
    }
}

/**
 * Run the parameterized test suite "SettingsTests" with these parameter
 * combinations. The first item of the tuple is the parameter name, the
 * second is its value, stored as a rr::Variant. It is sometimes useful
 * to run these one at a time. To do so, just temporarily comment out
 * the ones you dont want to run.
 */
INSTANTIATE_TEST_SUITE_P(
        KinsolSteadySetateSolverSettingsTests,
        SettingsTests,
        ::testing::Values(
                std::pair<std::string, Variant>("Step", Variant(1)),
                std::pair<std::string, Variant>("fnormtol", Variant(1.25e-4)),
                std::pair<std::string, Variant>("scsteptol", Variant(4.68e-8)),
                std::pair<std::string, Variant>("kinLogLevel", Variant(2)))
        );











