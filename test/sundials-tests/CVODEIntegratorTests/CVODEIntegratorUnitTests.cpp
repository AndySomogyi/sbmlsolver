
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "rrRoadRunner.h"
#include "rrExecutableModel.h"
#include "CVODEIntegrator.h"
#include "Solver.h"
#include "TestModelFactory.h"

using namespace rr;

class CVODEIntegratorUnitTests : public ::testing::Test {
public:

    CVODEIntegratorUnitTests() = default;

};


TEST_F(CVODEIntegratorUnitTests, SetConcentrationToleranceFromScalar) {
    RoadRunner rr(TestModelFactory("OpenLinearFlux")->str());
    Integrator *cvodeIntegrator = rr.getIntegrator();
    cvodeIntegrator->setConcentrationTolerance(0.1234);
    auto x = cvodeIntegrator->getConcentrationTolerance();
    for (auto i: x) {
        ASSERT_NEAR(i, 0.1234, 1e-7);
    }
}

TEST_F(CVODEIntegratorUnitTests, SetConcentrationToleranceFromVector) {
    RoadRunner rr(TestModelFactory("OpenLinearFlux")->str());
    Integrator *cvodeIntegrator = rr.getIntegrator();
    std::vector<double> d({0.5678, 0.4321});
    cvodeIntegrator->setConcentrationTolerance(d);
    ASSERT_EQ(d, cvodeIntegrator->getConcentrationTolerance());
}




















