
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

TEST_F(CVODEIntegratorUnitTests, SetConcentrationToleranceFromScalarSmallValue) {
    RoadRunner rr(TestModelFactory("OpenLinearFlux")->str());
    Integrator *cvodeIntegrator = rr.getIntegrator();
    cvodeIntegrator->setConcentrationTolerance(1e-15);
    auto x = cvodeIntegrator->getConcentrationTolerance();
    for (auto i: x) {
        ASSERT_NEAR(i, 1e-15, 1e-7);
    }
}

TEST_F(CVODEIntegratorUnitTests, SetConcentrationToleranceFromVector) {
    RoadRunner rr(TestModelFactory("OpenLinearFlux")->str());
    Integrator *cvodeIntegrator = rr.getIntegrator();
    std::vector<double> d({0.5678, 0.4321});
    cvodeIntegrator->setConcentrationTolerance(d);
    ASSERT_EQ(d, cvodeIntegrator->getConcentrationTolerance());
}

TEST_F(CVODEIntegratorUnitTests, SetAmountToleranceFromScalar) {
    RoadRunner rr(TestModelFactory("OpenLinearFlux")->str());
    Integrator *cvodeIntegrator = rr.getIntegrator();
    cvodeIntegrator->setValue("absolute_tolerance", 1e-15);
    std::vector<double> absTolVector = cvodeIntegrator->getConcentrationTolerance();
    for (auto it : absTolVector ) {
        ASSERT_NEAR(it, 1e-15, 1e-10);
    }
}

TEST_F(CVODEIntegratorUnitTests, SetAmountToleranceFromVector) {
    RoadRunner rr(TestModelFactory("OpenLinearFlux")->str());
    Integrator *cvodeIntegrator = rr.getIntegrator();
    std::vector<double> input({1e-14, 1e-16});
    cvodeIntegrator->setValue("absolute_tolerance", input);
    std::vector<double> absTolVector = cvodeIntegrator->getValue("absolute_tolerance");
    ASSERT_EQ(input, absTolVector);
}


















