#include <fstream>
#include "gtest/gtest.h"

#include "NewtonIteration.h"
#include "rrRoadRunner.h"
#include "SBMLTestModelFactory.h"

using namespace rr;

//todo remember to delete this method before merge with develop.
void openInCopasi(const std::string &sbml) {
    std::string f = R"(D:\roadrunner\roadrunner\test\sundials-tests\SundialsSteadyStateTests\tmp.cps)";
    std::ofstream cps;
    cps.open(f);
    cps << sbml;
    cps.close();
    std::string cmd = "CopasiUI -i " + f;
    system(cmd.c_str());
}

/**
 */
class NewtonIterationIntegrationTests : public ::testing::Test {

public:
    NewtonIterationIntegrationTests() = default;

    // todo pull this into superclass, as other types of solver will also need it.
    template<class TestModelType>
    void testSteadyState(const std::string &modelName, bool useMoietyConservation = false,
                         const std::string &strategy = "basic") {
        // get the model
        std::unique_ptr<SBMLTestModel> testModelPtr = SBMLTestModelFactory(modelName);
        auto testModel = std::unique_ptr<TestModelType>(dynamic_cast<TestModelType*>(testModelPtr.release()));

        assert(testModel && "testModel is nullptr");

        // load it into rr
        RoadRunner rr(testModel->str());

        // collect reference results
        ResultMap expectedResult = testModel->steadyState();

        // insert starting values
        for (auto &it: expectedResult){
            const std::string& speciesName = it.first;
            const double& startingValue = it.second.first;
            std::cout << "Setting \"" << speciesName << "\" to " << startingValue << std::endl;
            rr.setInitConcentration(speciesName, startingValue, false);
        }
        rr.regenerate();

        rr.setSteadyStateSolver("NewtonIteration");

        // add any settings specific for this problem
        BasicDictionary steadyStateOptions;
        steadyStateOptions.setItem("strategy", strategy); // injected strategy here
        steadyStateOptions.setItem("PrintLevel", 3);

        for (auto &settingsIterator : testModel->settings()){
            steadyStateOptions.setItem(settingsIterator.first, Variant(settingsIterator.second));
        }

        // turn on/off conservation analysis
        rr.setConservedMoietyAnalysis(useMoietyConservation);

        std::cout << "rr.getFullStoichiometryMatrix" << std::endl;
        std::cout << rr.getFullStoichiometryMatrix().numRows() << "x" <<
        rr.getFullStoichiometryMatrix().numCols()<< std::endl;

        std::cout << "rr.getReducedStoichiometryMatrix" << std::endl;
        std::cout << rr.getReducedStoichiometryMatrix().numRows() << "x" <<
        rr.getReducedStoichiometryMatrix().numCols()<< std::endl;

        std::cout << "rr.getFullJacobian" << std::endl;
        std::cout << rr.getFullJacobian() << std::endl;

        std::cout << "rr.getReducedJacobian" << std::endl;
        std::cout << rr.getReducedJacobian() << std::endl;

        /*
            [[0,0,0,-13.4333,382.691,0,0,0,0,0,0,0,0],
            [0,-14.2784,0,0,0,0,0,0,0,0,0,0,0],
            [0,2.30835,-0.655209,0,0,0,34.4364,0,0,0,0,0,0],
            [0,0,0,-220.307,0,0,0,0,0,0,0,0,0],
            [0,0,0,0,-0.0716435,0,0,0,0,0,0,0,0],
            [24.8873,0,0,0,0,-126.447,0,0,0,0,0,0,0],
            [0,0,0,0,0,0,-4.86736,0,0,0.231518,0,0,0],
            [0,0,0.553919,0,0,0,0,-0.305147,0,0,0,0,0],
            [0,0,0,-4.2007,0,0.0236007,0,0,0,0,0,0,0],
            [0,0,0,0,0,0,0,0,0,-11.0538,0,0,1.28411],
            [0,0,0,0,0,0,0,0.00290243,0,0,0,0,0],
            [0,0,0,0,0,0,0,0,0,0,0,0,0],
            [0,0,0,0,132.676,0,0,0,0,0,0,0,0]]
         */

        rr.steadyState(&steadyStateOptions);

        NewtonIteration * newtonIteration = dynamic_cast<NewtonIteration*>(
                rr.getSteadyStateSolver()
        );
        newtonIteration->printSolverStats();

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
        //openInCopasi(testModel->str());
    }

};

/**
 * @brief tests in this fixture all solve for steady state using the
 * "basic" strategy. This corresponds to setting the KIN_NONE flag
 * in kinsol.
 */
class BasicNewtonIterationTests : public NewtonIterationIntegrationTests {
public:
    std::string strategy = "basic";

    BasicNewtonIterationTests() {
//        Logger::setLevel(Logger::LOG_DEBUG);
    };
};

TEST_F(BasicNewtonIterationTests, CheckCorrectSteadyStateOpenLinearFlux) {
    testSteadyState<OpenLinearFlux>(
            "OpenLinearFlux",
            false,
            strategy
            );
}

TEST_F(BasicNewtonIterationTests, CheckCorrectSteadyStateSimpleFluxManuallyReduced) {
    testSteadyState<SimpleFluxManuallyReduced>("SimpleFluxManuallyReduced",
                                               false,  // moiety conservation done manually
                                               strategy
    );
}

TEST_F(BasicNewtonIterationTests, CheckCorrectSteadyStateSimpleFlux) {
    testSteadyState<SimpleFlux>("SimpleFlux", true, strategy);
}

//TEST_F(BasicNewtonIterationTests, CheckCorrectSteadyStateVenkatraman2010) {
//    testSteadyState<Venkatraman2010>("Venkatraman2010", false, strategy, true, 5);
//}

/**
 * The reduced jacobian for the Brown2004 model is singular. This is the matrix:
 *
 * j = np.array([[0,0,0,-13.4333,382.691,0,0,0,0,0,0,0,0],^M
 *           [0,-14.2784,0,0,0,0,0,0,0,0,0,0,0],^M
 *           [0,2.30835,-0.655209,0,0,0,34.4364,0,0,0,0,0,0],^M
 *           [0,0,0,-220.307,0,0,0,0,0,0,0,0,0],^M
 *           [0,0,0,0,-0.0716435,0,0,0,0,0,0,0,0],^M
 *           [24.8873,0,0,0,0,-126.447,0,0,0,0,0,0,0],^M
 *           [0,0,0,0,0,0,-4.86736,0,0,0.231518,0,0,0],^M
 *           [0,0,0.553919,0,0,0,0,-0.305147,0,0,0,0,0],^M
 *           [0,0,0,-4.2007,0,0.0236007,0,0,0,0,0,0,0],^M
 *           [0,0,0,0,0,0,0,0,0,-11.0538,0,0,1.28411],^M
 *           [0,0,0,0,0,0,0,0.00290243,0,0,0,0,0],^M
 *           [0,0,0,0,0,0,0,0,0,0,0,0,0],^M
 *           [0,0,0,0,132.676,0,0,0,0,0,0,0,0]])
 *
 *  But, copasi can solve this problem. So what's going on?
 */
TEST_F(BasicNewtonIterationTests, CheckCorrectSteadyStateBrown2004) {
    testSteadyState<Brown2004>("Brown2004", true, strategy);
}


/**
 * @brief tests in this fixture all solve for steady state using the
 * "linesearch" strategy. This corresponds to setting the KIN_LINESEARCH flag
 * in kinsol.
 */
class LineSearchNewtonIterationTests : public NewtonIterationIntegrationTests {
public:
    std::string strategy = "linesearch";

    LineSearchNewtonIterationTests() = default;
};

TEST_F(LineSearchNewtonIterationTests, CheckCorrectSteadyStateOpenLinearFlux) {
    testSteadyState<OpenLinearFlux>("OpenLinearFlux", false, strategy);
}

TEST_F(LineSearchNewtonIterationTests, CheckCorrectSteadyStateSimpleFluxManuallyReduced) {
    testSteadyState<SimpleFluxManuallyReduced>("SimpleFluxManuallyReduced",
                                               false, // moiety conservation done manually
                                               strategy);
}

TEST_F(LineSearchNewtonIterationTests, CheckCorrectSteadyStateSimpleFlux) {
    testSteadyState<SimpleFlux>("SimpleFlux", true, strategy);
}

//TEST_F(LineSearchNewtonIterationTests, CheckCorrectSteadyStateVenkatraman2010) {
//    testSteadyState<Venkatraman2010>("Venkatraman2010", false, strategy, true, 3);
//}


//TEST_F(LineSearchNewtonIterationTests, CheckCorrectSteadyStateBrown2004) {
//    testSteadyState<Brown2004>("Brown2004", true, strategy);
//}






















