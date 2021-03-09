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
 * Takes test model name as argument
 */
class NewtonIterationIntegrationTests : public ::testing::Test {

public:
    NewtonIterationIntegrationTests() = default;

    // todo pull this into superclass, as other types of solver will also need it.
    template<class TestModelType>
    void testSteadyState(const std::string &modelName, bool useMoietyConservation = false,
                         const std::string &strategy = "basic",
                         bool presimulation = false, double presimulationEndTime = 1 ) {
        // get the model
        std::unique_ptr<SBMLTestModel> testModelPtr = SBMLTestModelFactory(modelName);
        auto testModel = std::unique_ptr<TestModelType>(dynamic_cast<TestModelType*>(testModelPtr.release()));

        assert(testModel && "testModel is nullptr");

        // load it into rr
        RoadRunner rr(testModel->str());
//        RoadRunner rr("C:\\Users\\Ciaran\\Downloads\\BIOMD0000000033_url.xml");

        // collect reference results
        ResultMap expectedResult = testModel->steadyState();

        for (auto &it: expectedResult){
            const std::string& speciesName = it.first;
            const double& startingValue = it.second.first;
            rr.setInitConcentration(speciesName, startingValue, false);
        }
        rr.regenerate();

        //openInCopasi(rr.getSBML());

        // turn on/off conservation analysis
        rr.setConservedMoietyAnalysis(useMoietyConservation);

        // instantiate our solver
        NewtonIteration newtonIteration(rr.getModel());

        if (presimulation){
            newtonIteration.setValue("allow_presimulation", presimulation);
            newtonIteration.setValue("presimulation_time", presimulationEndTime);
        }

        // set some parameters
        newtonIteration.setValue("strategy", strategy);
        newtonIteration.setValue("PrintLevel", 3);

        newtonIteration.setValue("FuncNormTol", 1e-15);

        // openInCopasi(testModel->str());
        // solve the problem
        double res = newtonIteration.solve();

        newtonIteration.printSolverStats();

        std::cout << "res: " << res << std::endl;

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
    testSteadyState<OpenLinearFlux>(
            "OpenLinearFlux",
            false,
            strategy,
            true, 1
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

TEST_F(BasicNewtonIterationTests, CheckCorrectSteadyStateBrown2004) {
    testSteadyState<Brown2004>("Brown2004", false, strategy, false, 1);
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
    testSteadyState<OpenLinearFlux>("OpenLinearFlux", false, strategy, true, 1);
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























