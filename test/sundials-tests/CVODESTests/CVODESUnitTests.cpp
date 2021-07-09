//
// Created by Ciaran Welsh on 09/06/2021.
//

#include "gtest/gtest.h"
#include "TestModelFactory.h"
#include "ForwardSensitivitySolver.h"

using namespace rr;

class CVODESUnitTests : public ::testing::Test {
public:
    RoadRunner rr;
    ExecutableModel* model;
    CVODESUnitTests() {
        rr.load(SimpleFlux().str());
        model = rr.getModel();
    }
};

TEST_F(CVODESUnitTests, getGlobalParameterNames){
    ForwardSensitivitySolver forwardSensitivitySolver(model);
    auto names = forwardSensitivitySolver.getGlobalParameterNames();
    std::vector<std::string> expected({"kf", "kb"});
    ASSERT_EQ(expected, names);
}

TEST_F(CVODESUnitTests, getModelParametersAsMap){
    ForwardSensitivitySolver forwardSensitivitySolver(model);
    auto m = forwardSensitivitySolver.getModelParametersAsMap();
    std::unordered_map<std::string, double> expected{
            {"kf", 0.1},
            {"kb", 0.01},
    };
    ASSERT_EQ(expected, m);
}


TEST_F(CVODESUnitTests, getModelParametersAsVector){
    ForwardSensitivitySolver forwardSensitivitySolver(model);
    auto v = forwardSensitivitySolver.getModelParametersAsVector();
    std::vector<double> expected({0.1, 0.01});
    ASSERT_EQ(expected, v);
}


TEST_F(CVODESUnitTests, deducePlistDefaultToAllParameters){
    ForwardSensitivitySolver forwardSensitivitySolver(model);
    forwardSensitivitySolver.deducePlist();
    std::vector<int> expected({0, 1});
    ASSERT_EQ(expected, forwardSensitivitySolver.plist);
}

TEST_F(CVODESUnitTests, deducePlistFirstParameter){
    ForwardSensitivitySolver forwardSensitivitySolver(model, {"kf"});
    forwardSensitivitySolver.deducePlist();
    std::vector<int> expected({0});
    ASSERT_EQ(expected, forwardSensitivitySolver.plist);
}

TEST(CVODES, deducePlistSecondParameter){
    RoadRunner r(SimpleFlux().str());
    ExecutableModel* model = r.getModel();
    ForwardSensitivitySolver forwardSensitivitySolver(model, {"kb"});
    forwardSensitivitySolver.deducePlist();
    std::vector<int> expected({1});
    ASSERT_EQ(expected, forwardSensitivitySolver.plist);
}





























