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
        double expected = testModel.steadyState()[speciesID]; // first is start val, second is speciesID at steady state
        std::cout << "Comparing \"" << speciesID << "\" expected result: " << expected
                  << " with actual result " << actualResult << std::endl;
        EXPECT_NEAR(expected, actualResult, 0.0001);
    }
}
void SundialsSteadyStateSolverUnitTest::checkResults(StringDoubleMap expectedMap, ls::DoubleMatrix result) {
    std::vector<std::string> names = result.getColNames();
    for (int i = 0; i < names.size(); i++) {
        std::string speciesID = names[i];
        double actualResult = result[0][i]; // 0th row, ith col of a DoubleMatrix
        double expected = expectedMap[speciesID];
        std::cout << "Comparing \"" << speciesID << "\" expected result: " << expected
                  << " with actual result " << actualResult << std::endl;
        EXPECT_NEAR(expected, actualResult, 0.0001);
    }
}

void SundialsSteadyStateSolverUnitTest::checkResults(ls::DoubleMatrix expectedMatrix, ls::DoubleMatrix actualMatrix) {
    if (expectedMatrix.numRows() != actualMatrix.numRows()){
        std::cerr << "number of rows in expected Vs actual are not equal" << std::endl;
        ASSERT_EQ(expectedMatrix.numRows(), actualMatrix.numRows());
    }

    if (expectedMatrix.numCols() != actualMatrix.numCols()){
        std::cerr << "number of cols in expected Vs actual are not equal" << std::endl;
        ASSERT_EQ(expectedMatrix.numCols(), actualMatrix.numCols());
    }

    for (int i=0; i< expectedMatrix.numRows(); i++){
        for (int j=0; j<expectedMatrix.numCols(); j++){
            std::cout << "Comparing expected result: " << expectedMatrix[i][j]
                <<" with actual result: " << actualMatrix[i][j] << std::endl;
            EXPECT_NEAR(expectedMatrix[i][j], actualMatrix[i][j], 0.001);
        }
    }
}
