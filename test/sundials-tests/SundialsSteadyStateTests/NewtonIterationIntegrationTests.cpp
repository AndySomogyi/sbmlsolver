#include <fstream>
#include "gtest/gtest.h"

#include "NewtonIteration.h"
#include "rrRoadRunner.h"
#include "SBMLTestModelFactory.h"

using namespace rr;

//todo remember to delete this method.
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
 * Takes test model name as argument
 */
class NewtonIterationIntegrationTests : public ::testing::Test {

public:
    NewtonIterationIntegrationTests() = default;

    // todo pull this into superclass, as other types of solver will also need it.
    template<class TestModelType>
    void testSteadyState(const std::string &modelName, bool useMoietyConservation = false,
                         const std::string &strategy = "basic") {
        // get the model
        auto *testModel = (TestModelType *) SBMLTestModelFactory(modelName);

        // load it into rr
        RoadRunner rr(testModel->str());

        // turn on/off conservation analysis
        rr.setConservedMoietyAnalysis(useMoietyConservation);
//        std::cout << rr.getFullJacobian() << std::endl;
//        std::cout << rr.getFullStoichiometryMatrix() << std::endl;
//        std::cout << rr.getReducedJacobian() << std::endl;
//        std::cout << rr.getReducedStoichiometryMatrix() << std::endl;

//        std::cout << "link matrix : " << std::endl;
//        if (useMoietyConservation)
//            std::cout << rr.getConservationMatrix() << std::endl;

        // instantiate our solver
        NewtonIteration newtonIteration(rr.getModel());

        // collect reference results
        ResultMap expectedResult = testModel->steadyState();
        // set the model parameters for this run
//        for (const auto &parameterInfo : expectedResult) {
//            std::string speciesName = parameterInfo.first;
//            double startingValue = parameterInfo.second.first; // second is a std::pair of doubles
//            rr.setInitConcentration(speciesName, startingValue, false);
//        }
//        rr.regenerate();

        // set some parameters
        newtonIteration.setValue("fScaleDefault", 0.1);
        newtonIteration.setValue("uScaleDefault", 0.1);
        newtonIteration.setValue("strategy", strategy);

        //openInCopasi(testModel->str());
        // solve the problem
        newtonIteration.solve();

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
    }


    template<class TestModelType>
    void testSteadyStateMultiStart(const std::string &modelName, bool useMoietyConservation = false,
                                   const std::string &strategy = "basic") {
        // get the model
        auto *testModel = (TestModelType *) SBMLTestModelFactory(modelName);

        // load it into rr
        RoadRunner rr(testModel->str());

        // turn on/off conservation analysis
        rr.setConservedMoietyAnalysis(useMoietyConservation);
//        std::cout << rr.getFullJacobian() << std::endl;
//        std::cout << rr.getFullStoichiometryMatrix() << std::endl;
//        std::cout << rr.getReducedJacobian() << std::endl;
//        std::cout << rr.getReducedStoichiometryMatrix() << std::endl;

//        std::cout << "link matrix : " << std::endl;
//        if (useMoietyConservation)
//            std::cout << rr.getConservationMatrix() << std::endl;

        // instantiate our solver
        NewtonIteration newtonIteration(rr.getModel());

        // iterate over reference results
        for (ResultMap &steadyStateResult: testModel->steadyState()) {
            // ensure we start from a 'clean' model
            rr.reset();

            // set the model parameters for this run
            for (const auto &parameterInfo : steadyStateResult) {
                std::string speciesName = parameterInfo.first;
                double startingValue = parameterInfo.second.first; // second is a std::pair of doubles
                rr.setInitConcentration(speciesName, startingValue, false);
            }
            rr.regenerate();

            // set some parameters
            newtonIteration.setValue("fScaleDefault", 0.1);
            newtonIteration.setValue("uScaleDefault", 0.1);
            newtonIteration.setValue("strategy", strategy);

            std::cout << rr.getSBML() << std::endl;


            //openInCopasi(testModel->str());
            // solve the problem
            newtonIteration.solve();

            // collect actual results from model
            auto result = rr.getFloatingSpeciesConcentrationsNamedArray();
            std::vector<std::string> names = result.getColNames();

            // check to see if actual result are near expected.
            for (int i = 0; i < names.size(); i++) {
                std::string speciesID = names[i];
                double expectedResult = steadyStateResult[speciesID].second;
                double actualResult = result[0][i];

                std::cout << "Comparing \"" << speciesID << "\" expected result: " << expectedResult
                          << " with actual result " << actualResult << std::endl;
                EXPECT_NEAR(expectedResult, actualResult, 0.0001);
            }
        }
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

    BasicNewtonIterationTests() = default;
};

TEST_F(BasicNewtonIterationTests, CheckCorrectSteadyStateOpenLinearFlux) {
    testSteadyState<OpenLinearFlux>("OpenLinearFlux", false, strategy);
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

TEST_F(BasicNewtonIterationTests, CheckCorrectSteadyStateVenkatraman2010) {
    testSteadyStateMultiStart<Venkatraman2010>("Venkatraman2010", false, strategy);
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

TEST_F(LineSearchNewtonIterationTests, CheckCorrectSteadyStateVenkatraman2010) {
    testSteadyStateMultiStart<Venkatraman2010>("Venkatraman2010", false, strategy);
}












// Need to write a function to make a call to lapack's dgetrf
// function for computing the determinent.
// Then compute it for the jacobian of this system.
// This operation is potentially computationally intensive,
// so it should be possible to turn this check off.
// However, if it is on, and we find that the determinent is 0,
// (i.e. we have a singular matrix), then we automatically turn on
// moiety conservation analysis, which should* fix the error.
//
// But the question remains, how does moiety conservation analysis reduce the system
// in such a way as to bypass the problem?


// Also, todo: must consider whether stiochoimetry is already considered when
// roadrunner gives us the vector of rates.


// todo test the same model from different initial conditions. all 0's seems to be a problem.
























