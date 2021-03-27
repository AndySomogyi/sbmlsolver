#include "gtest/gtest.h"
#include "TestModelFactory.h"
#include "rrRoadRunner.h"

using namespace rr;

/**
 * Integration style tests verifying interactions between
 * steady state decorators.
 */
class DecoratorIntegrationTests : public ::testing::Test {

public:

    DecoratorIntegrationTests() = default;

    void checkResult(OpenLinearFlux &testModel, RoadRunner &rr) {
        const auto &expectedResult = testModel.steadyState();
        for (auto &r: expectedResult) {
            const auto &speciesName = r.first;
            const auto &expectedValue = r.second;
            const auto &actualValue = rr.getValue(speciesName);
            ASSERT_NEAR(expectedValue, actualValue, 1e-5);
        }
    }

};


/**
 * This model cannot be solved without presimulation or approximation
 */
TEST_F(DecoratorIntegrationTests, NoDecoratorsAdded) {
    OpenLinearFlux testModel;
    RoadRunner rr(testModel.str());
    rr.setSteadyStateSolver("newton");

    BasicDictionary opt;
    opt.setItem("allow_approx", false);
    opt.setItem("allow_presimulation", false);

    ASSERT_THROW(rr.steadyState(&opt), std::runtime_error);
}

TEST_F(DecoratorIntegrationTests, PresimulationOnly) {
    OpenLinearFlux testModel;
    RoadRunner rr(testModel.str());
    rr.setSteadyStateSolver("newton");

    BasicDictionary opt;
    opt.setItem("allow_approx", false);
    opt.setItem("allow_presimulation", true);

    rr.steadyState(&opt);
    checkResult(testModel, rr);
}

TEST_F(DecoratorIntegrationTests, ApproximationOnly) {
    OpenLinearFlux testModel;
    RoadRunner rr(testModel.str());
    rr.setSteadyStateSolver("newton");

    BasicDictionary opt;
    opt.setItem("allow_approx", true);
    opt.setItem("allow_presimulation", false);

    rr.steadyState(&opt);
    checkResult(testModel, rr);

}

/**
 *
 */
TEST_F(DecoratorIntegrationTests, CombineApproxAndPresimDecorators) {
    // first try presimulation then will try to approximate
    OpenLinearFlux testModel;
    RoadRunner rr(testModel.str());
    rr.setSteadyStateSolver("newton");

    BasicDictionary opt;
    opt.setItem("allow_approx", true);
    opt.setItem("allow_presimulation", true);

    // this is *not* enough presimulation for the OpenLinearFlux model to solve
    // therefore the algorithm is force to turn to approximation, which it does automatically.
    opt.setItem("presimulation_time", 0.0001);

    rr.steadyState(&opt);
    checkResult(testModel, rr);
}






