//
// Created by Ciaran on 24/03/2021.
//

#include "SundialsSteadyStateSolverUnitTest.h"


SundialsSteadyStateSolverUnitTest::SundialsSteadyStateSolverUnitTest() {
    rr = new RoadRunner(testModel.str());
}

SundialsSteadyStateSolverUnitTest::~SundialsSteadyStateSolverUnitTest() {
    delete rr;
}

void SundialsSteadyStateSolverUnitTest::checkResults(ls::DoubleMatrix result) {
    std::vector<std::string> names = result.getColNames();
    for (int i = 0; i < names.size(); i++) {
        std::string speciesID = names[i];
        double actualResult = result[0][i]; // 0th row, ith col of a DoubleMatrix
        double expected = testModel.steadyState()[speciesID].second; // first is start val, second is speciesID at steady state
        std::cout << "Comparing \"" << speciesID << "\" expected result: " << expected
                  << " with actual result " << actualResult << std::endl;
        EXPECT_NEAR(expected, actualResult, 0.0001);
    }
}
