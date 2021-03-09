

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

    void syncWithModel(ExecutableModel *m) override {
        // although this does no extra work compared
        // to superclass implementation, we need it
        // in this class, so the right implementations
        // of freeKinsol and createKinsol get called.
        //   --> otherwise memory leaks or worse
        KinsolSteadyStateSolver::syncWithModel(m);
    }

    void freeKinsol() override {};

    void updateKinsol() override {};

    void resetSettings() override{
        KinsolSteadyStateSolver::resetSettings();
    }

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











