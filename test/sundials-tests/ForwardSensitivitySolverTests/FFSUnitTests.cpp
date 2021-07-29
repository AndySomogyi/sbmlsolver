//
// Created by Ciaran Welsh on 09/06/2021.
//

#include "gtest/gtest.h"
#include "TestModelFactory.h"
#include "ForwardSensitivitySolver.h"

using namespace rr;

class FFSUnitTests : public ::testing::Test {
public:
    RoadRunner rr;
    ExecutableModel *model;

    FFSUnitTests() {
        rr.load(SimpleFlux().str());
        model = rr.getModel();
    }
};

TEST_F(FFSUnitTests, getGlobalParameterNames) {
    ForwardSensitivitySolver forwardSensitivitySolver(model);
    auto names = forwardSensitivitySolver.getGlobalParameterNames();
    std::vector<std::string> expected({"kf", "kb"});
    ASSERT_EQ(expected, names);
}

TEST_F(FFSUnitTests, getVariableNames) {
    ForwardSensitivitySolver forwardSensitivitySolver(model);
    auto names = forwardSensitivitySolver.getVariableNames();
    std::vector<std::string> expected({"S1", "S2"});
    ASSERT_EQ(expected, names);
}

TEST_F(FFSUnitTests, CheckValueOfNp) {
    ForwardSensitivitySolver forwardSensitivitySolver(model);
    ASSERT_EQ(2, forwardSensitivitySolver.Np);
}

TEST_F(FFSUnitTests, CheckValueOfNsWhenDefaultToAllParameters) {
    ForwardSensitivitySolver forwardSensitivitySolver(model);
    ASSERT_EQ(2, forwardSensitivitySolver.Ns);
}

TEST_F(FFSUnitTests, CheckValueOfNsWhenSelectingParameters) {
    ForwardSensitivitySolver forwardSensitivitySolver(model, {"kf"});
    ASSERT_EQ(1, forwardSensitivitySolver.Ns);
}

TEST_F(FFSUnitTests, CheckValueOfNumModelVariables) {
    ForwardSensitivitySolver forwardSensitivitySolver(model);
    ASSERT_EQ(2, forwardSensitivitySolver.numModelVariables);
}

TEST_F(FFSUnitTests, getModelParametersAsMap) {
    ForwardSensitivitySolver forwardSensitivitySolver(model);
    auto m = forwardSensitivitySolver.getModelParametersAsMap();
    std::unordered_map<std::string, double> expected{
            {"kf", 0.1},
            {"kb", 0.01},
    };
    ASSERT_EQ(expected, m);
}

TEST_F(FFSUnitTests, getModelParametersAsVector) {

    ForwardSensitivitySolver forwardSensitivitySolver(model);
    auto v = forwardSensitivitySolver.getModelParametersAsVector();
    std::vector<double> expected({0.1, 0.01});
    ASSERT_EQ(expected, v);
}

TEST_F(FFSUnitTests, deducePlistDefaultToAllParameters) {
    ForwardSensitivitySolver forwardSensitivitySolver(model);
    std::vector<int> expected({0, 1});
    ASSERT_EQ(expected, forwardSensitivitySolver.plist);
}

TEST_F(FFSUnitTests, deducePlistFirstParameter) {
    ForwardSensitivitySolver forwardSensitivitySolver(model, {"kf"});
    std::vector<int> expected({0});
    ASSERT_EQ(expected, forwardSensitivitySolver.plist);
}

TEST_F(FFSUnitTests, deducePlistSecondParameter) {
    RoadRunner r(SimpleFlux().str());
    ExecutableModel *model = r.getModel();
    ForwardSensitivitySolver forwardSensitivitySolver(model, {"kb"});
    std::vector<int> expected({1});
    ASSERT_EQ(expected, forwardSensitivitySolver.plist);
}

TEST_F(FFSUnitTests, RunTwoSensitivityAnalysesBackToBack) {
    RoadRunner r(SimpleFlux().str());
    ExecutableModel *model = r.getModel();
    ForwardSensitivitySolver forwardSensitivitySolver(model);
    forwardSensitivitySolver.solveSensitivities(0, 10, 11);
    forwardSensitivitySolver.solveSensitivities(0, 10, 11);
    // if the second call to getSensitivityMatrix() doesn't throw then we've passed
    ASSERT_NO_THROW(forwardSensitivitySolver.getSensitivityMatrix());
}

TEST_F(FFSUnitTests, AddParameterCheckNp) {
    RoadRunner r(SimpleFlux().str());
    r.addParameter("newP", 5, true);
    auto s = dynamic_cast<ForwardSensitivitySolver*>(r.getSensitivitySolver());
    ASSERT_EQ(3, s->Np);
}

TEST_F(FFSUnitTests, AddParameterCheckNs) {
    RoadRunner r(SimpleFlux().str());
    r.addParameter("newP", 5, true);
    auto s = dynamic_cast<ForwardSensitivitySolver*>(r.getSensitivitySolver());
    ASSERT_EQ(3, s->Ns);
}


TEST_F(FFSUnitTests, SettingsNonLinearSolver) {
    RoadRunner r(SimpleFlux().str());
    r.getSensitivitySolver()->setValue("nonlinear_solver", "fixed_point");
    auto s = r.getSensitivitySolver()->getValue("nonlinear_solver");
    ASSERT_STREQ(s.getAs<std::string>().c_str(), "fixed_point");
}

TEST_F(FFSUnitTests, ChangeSolverSettingAndRegenerateModel) {
    RoadRunner r(SimpleFlux().str());
    r.getSensitivitySolver()->setValue("nonlinear_solver", "fixed_point");
    r.regenerateModel();
    auto s = r.getSensitivitySolver()->getValue("nonlinear_solver");
    ASSERT_STREQ(s.getAs<std::string>().c_str(), "fixed_point");
}

TEST_F(FFSUnitTests, SolveSensitivitiesForSelectiveParameters1) {
    RoadRunner r(SimpleFlux().str());
    ExecutableModel *model = r.getModel();
    ForwardSensitivitySolver forwardSensitivitySolver(model, {"kf"});
    ASSERT_NO_THROW(forwardSensitivitySolver.solveSensitivities(0, 10, 11));
}


TEST_F(FFSUnitTests, SolveSensitivitiesForSelectiveParameters2) {
    RoadRunner r(SimpleFlux().str());
    ExecutableModel *model = r.getModel();
    ForwardSensitivitySolver forwardSensitivitySolver(model);
    forwardSensitivitySolver.solveSensitivities(0, 10, 11, {"kf"});
}

TEST_F(FFSUnitTests, SolveTwiceWithDifferentParametersBothTimes) {
    RoadRunner r(OpenLinearFlux().str());
    ExecutableModel *model = r.getModel();
    ForwardSensitivitySolver forwardSensitivitySolver(model);
    auto first = forwardSensitivitySolver.solveSensitivities(0, 10, 11, {"kf", "kout"});
    auto second = forwardSensitivitySolver.solveSensitivities(0, 10, 11, {"kin"});
    ASSERT_EQ(2, first.numRows());
    ASSERT_EQ(1, second.numRows());
}

TEST_F(FFSUnitTests, SolveTwiceWithDifferentParametersBothTimesEmptySecond) {
    RoadRunner r(OpenLinearFlux().str());
    ExecutableModel *model = r.getModel();
    ForwardSensitivitySolver forwardSensitivitySolver(model);
    auto first = forwardSensitivitySolver.solveSensitivities(0, 10, 11, {"kf", "kout"});
    // empty params should reset back to 4
    auto second = forwardSensitivitySolver.solveSensitivities(0, 10, 11);
    ASSERT_EQ(2, first.numRows());
    ASSERT_EQ(4, second.numRows());
}


























