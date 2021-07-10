//
// Created by Ciaran Welsh on 09/06/2021.
//

#include "gtest/gtest.h"
#include "rrRoadRunner.h"
#include "TestModelFactory.h"
#include "ForwardSensitivitySolver.h"
#include "ForwardSensitivitySolver.h"
#include "../CVODEIntegratorTests/CvodeIntegrationTest.h"

using namespace rr;

class FFSIntegrationTests : public CVODEIntegratorTests {
public:
    FFSIntegrationTests() = default;
};


TEST_F(FFSIntegrationTests, TestSimpleFlux) {
    SimpleFlux testModel;
    checkModelIntegrates<ForwardSensitivitySolver>(&testModel);
}

TEST_F(FFSIntegrationTests, OpenLinearFlux) {
    OpenLinearFlux testModel;
    checkModelIntegrates<ForwardSensitivitySolver>(&testModel);
}

TEST_F(FFSIntegrationTests, TestModel269) {
    Model269 testModel;
    checkModelIntegrates<ForwardSensitivitySolver>(&testModel);
}

TEST_F(FFSIntegrationTests, TestModel28) {
    Model28 testModel;
    checkModelIntegrates<ForwardSensitivitySolver>(&testModel);
}

TEST_F(FFSIntegrationTests, FactorialInRateLaw) {
    FactorialInRateLaw testModel;
    std::cout << testModel.str() << std::endl;
    checkModelIntegrates<ForwardSensitivitySolver>(&testModel);
}


//
//TEST_F(FFSIntegrationTests, UserInterface){
//    // this is how we want users to interact with roadrunner sensitivities:
//    RoadRunner rr(OpenLinearFlux().str());
//    /**
//     * SensivityResult is a container of ls::DoubleMatrix objects,
//     * indexed by time.
//     */
//    SensitivityResult res = rr.sensitivities(0, 10, 11); // with options
//}

//
//TEST_F(FFSIntegrationTests, DefaultSensivitySolver){
//    // this is how we want users to interact with roadrunner sensitivities:
//    RoadRunner rr(OpenLinearFlux().str());
//    CVODESIntegrator* sensitivities = rr.getSensitivitySolver();
//    if (!sensitivities){
//        std::cout << "SENSITIVITIES IS NULLPTR" << std::endl;
//    }
//    ASSERT_STREQ(sensitivitiestivities->getName().c_str(), "forward");
//}
//



























