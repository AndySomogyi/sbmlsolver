//
// Created by Ciaran on 26/03/2021.
//

#ifndef ROADRUNNER_STEADYSTATEINTEGRATIONTESTS_H
#define ROADRUNNER_STEADYSTATEINTEGRATIONTESTS_H

#include "gtest/gtest.h"
#include "TestModelFactory.h"
#include "rrRoadRunner.h"
#include "SteadyStateSolver.h"

using namespace rr;

class SteadyStateIntegrationTests : public ::testing::Test {

public:
    SteadyStateIntegrationTests() = default;

    template<class TestModelType>
    void testSteadyState(
            const std::string &modelName,
            const std::string& solverName) {
        // get the model
        TestModel* testModel_ = TestModelFactory(modelName);
        TestModelType* testModel = dynamic_cast<TestModelType*>(testModel_);

        assert(testModel && "testModel is nullptr");

        // load it into rr
        RoadRunner rr(testModel->str());

        // collect reference results
        ResultMap expectedResult = testModel->steadyState();

        // insert starting values
        for (auto &it: expectedResult) {
            const std::string &speciesName = it.first;
            const double &startingValue = it.second.first;
            std::cout << "Setting \"" << speciesName << "\" to " << startingValue << std::endl;
            rr.setInitConcentration(speciesName, startingValue, false);
        }
        rr.regenerate();

        rr.setSteadyStateSolver(solverName);

        BasicDictionary steadyStateOptions;
        steadyStateOptions.setItem("PrintLevel", 0);

        for (auto &settingsIterator : testModel->settings()) {
            if (settingsIterator.first == "moiety_conservation"){
                rr.setConservedMoietyAnalysis(settingsIterator.second);
            } else {
                try {
                    rr.getSteadyStateSolver()->setValue(settingsIterator.first, Variant(settingsIterator.second));
                } catch (std::exception& err){
                    // if solver does not have this option, that's okay
                    continue;
                }
            }
        }


        std::cout << "rr.getFullStoichiometryMatrix" << std::endl;
        std::cout << rr.getFullStoichiometryMatrix().numRows() << "x" <<
                  rr.getFullStoichiometryMatrix().numCols() << std::endl;

        std::cout << "rr.getReducedStoichiometryMatrix" << std::endl;
        std::cout << rr.getReducedStoichiometryMatrix().numRows() << "x" <<
                  rr.getReducedStoichiometryMatrix().numCols() << std::endl;

        std::cout << "rr.getFullJacobian" << std::endl;
        std::cout << rr.getFullJacobian() << std::endl;

        std::cout << "rr.getReducedJacobian" << std::endl;
        std::cout << rr.getReducedJacobian() << std::endl;
        //openInCopasi(testModel->str());

        rr.steadyState(&steadyStateOptions);

        //todo support this for all steady state algo
        // auto solver = rr.getSteadyStateSolver();
        //solver->printSolverStats();

        // collect actual results from model
        auto result = rr.getFloatingSpeciesConcentrationsNamedArray();
        std::vector<std::string> names = result.getColNames();

        // check to see if actual result are near expected.
        for (int i = 0; i < names.size(); i++) {
            std::string speciesID = names[i];
            double actualResult = result[0][i]; // 0th row, ith col of a DoubleMatrix
            double expected = expectedResult[speciesID].second; // first is start val, second is speciesID at steady state

            std::cout << "Comparing \"" << speciesID << "\" expected result: " << expected
                      << " with actual result " << actualResult << std::endl;
            EXPECT_NEAR(expected, actualResult, 0.0001);
        }

        delete testModel;
    }
};


#endif //ROADRUNNER_STEADYSTATEINTEGRATIONTESTS_H
