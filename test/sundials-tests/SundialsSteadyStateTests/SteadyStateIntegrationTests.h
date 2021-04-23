#include <fstream>
#include "gtest/gtest.h"

#include "NewtonIteration.h"
#include "BasicNewtonIteration.h"
#include "LinesearchNewtonIteration.h"
#include "rrRoadRunner.h"
#include "TestModelFactory.h"
#include <chrono>
using namespace std::chrono;
using namespace rr;using namespace std::chrono;



class SteadyStateIntegrationTests : public ::testing::Test {

public:
    SteadyStateIntegrationTests() = default;

    template<class TestModelType>
    void testSteadyState(
            const std::string &modelName,
            const std::string &solverName) {
        // get the model
        TestModel *testModel_ = TestModelFactory(modelName);
        TestModelType *testModel = dynamic_cast<TestModelType *>(testModel_);

        assert(testModel && "testModel is nullptr");

        // load it into rr
        RoadRunner rr(testModel->str());

        // collect reference results
        StringDoubleMap expectedResult = testModel->steadyState();

        rr.setSteadyStateSolver(solverName);

        BasicDictionary steadyStateOptions;
        steadyStateOptions.setItem("PrintLevel", 0);

        for (auto &settingsIterator : testModel->steadyStateSettings()) {
            if (settingsIterator.first == "moiety_conservation") {
                rr.setConservedMoietyAnalysis(settingsIterator.second);
            } else {
                try {
                    rr.getSteadyStateSolver()->setValue(settingsIterator.first, Variant(settingsIterator.second));
                } catch (std::exception &err) {
                    // if solver does not have this option, that's okay
                    continue;
                }
            }
        }

        rr.steadyState(&steadyStateOptions);

        // collect actual results from model
        auto result = rr.getFloatingSpeciesConcentrationsNamedArray();
        std::vector<std::string> names = result.getColNames();

        // check to see if actual result are near expected.
        for (int i = 0; i < names.size(); i++) {
            std::string speciesID = names[i];
            double actualResult = result[0][i]; // 0th row, ith col of a DoubleMatrix
            double expected = expectedResult[speciesID]; // first is start val, second is speciesID at steady state

            std::cout << "Comparing \"" << speciesID << "\" expected result: " << expected
                      << " with actual result " << actualResult << std::endl;
            EXPECT_NEAR(expected, actualResult, 0.0001);
        }
        delete testModel;
    }
};












