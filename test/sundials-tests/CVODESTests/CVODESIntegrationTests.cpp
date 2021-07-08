//
// Created by Ciaran Welsh on 09/06/2021.
//

#include "gtest/gtest.h"
#include "rrRoadRunner.h"
#include "TestModelFactory.h"
#include "CvodesIntegrator.h"
#include "ForwardSensitivitySolver.h"
#include "CvodeIntegrationTest.h"

using namespace rr;

class CVODESIntegrationTests : public ::testing::Test {
public:
    CVODESIntegrationTests() = default;
};

//
//TEST_F(CVODESIntegrationTests, UserInterface){
//    // this is how we want users to interact with roadrunner sensitivities:
//    RoadRunner rr(OpenLinearFlux().str());
//    /**
//     * SensivityResult is a container of ls::DoubleMatrix objects,
//     * indexed by time.
//     */
//    SensitivityResult res = rr.sensitivities(0, 10, 11); // with options
//}


TEST_F(CVODESIntegrationTests, DefaultSensivitySolver){
    // this is how we want users to interact with roadrunner sensitivities:
    RoadRunner rr(OpenLinearFlux().str());
    CvodesIntegrator* sensitivities = rr.getSensitivitySolver();
    if (!sensitivities){
        std::cout << "SENSITIVITIES IS NULLPTR" << std::endl;
    }
    ASSERT_STREQ(sensitivitiestivities->getName().c_str(), "forward");
}




























