
#include <rrRoadRunner.h>
#include <NewtonIteration.h>
#include "gtest/gtest.h"

#include "Presimulation.h"
#include "SBMLTestModelFactory.h"

using namespace rr;

class PresimulationTests : public ::testing::Test {
public:
    std::unique_ptr<SBMLTestModel> testModel = nullptr;
    PresimulationTests() {
        testModel = SBMLTestModelFactory("OpenLinearFlux");
    };
};

TEST_F(PresimulationTests, t){
    RoadRunner rr(testModel->str());
    NewtonIteration newtonIteration(rr.getModel());

    Presimulation presimulation(rr.getModel(), &newtonIteration);

    presimulation.simulate();


    //Presimulation presimulation;
}

// we could then have a presimulation strategy parameter?
// 1, 10, 100,
// provide lambda func for user defined strategy

// okay what options does presimulation need to accomodate?


//TEST_F(NewtonIterationUnitTests, TestPresimulation) {
//    ls::DoubleMatrix before = rr.getFloatingSpeciesConcentrationsNamedArray();
//    Presimulation presimulation(rr.getModel(), 10, 1000, false);
//    presimulation.simulate();
//    ls::DoubleMatrix after = rr.getFloatingSpeciesConcentrationsNamedArray();
//    // it is the domain of CVODEIntegrator tests to ensure model
//    // is integrated correctly. Here, we only care that state vector
//    // before and after are different.
//
//    // No equality operators in ls::DoubleMatrix so we need to do it ourselves. Sad face =[
//    for (int i = 0; i < before.numRows(); i++) {
//        for (int j = 0; j < before.numCols(); j++) {
//            std::cout << "comparing before: " << before(i, j) << " to after: " << after(i, j) << std::endl;
//            ASSERT_NE(before(i, j), after(i, j));
//        }
//    }
//}