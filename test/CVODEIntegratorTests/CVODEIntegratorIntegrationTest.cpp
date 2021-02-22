

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "rrRoadRunner.h"
#include "rrExecutableModel.h"
#include "CVODEIntegrator.h"
#include "Solver.h"
#include "SBMLTestModelFactory.h"

using namespace rr;
using namespace testing;


/**
 * Parameterised fixture for passing models through a series of tests.
 * The template parameter is a string and is used as input into SBMLFactory.
 */
class CVODEIntegratorIntegrationTests : public ::testing::TestWithParam<std::string> {
public:

    CVODEIntegratorIntegrationTests() {
//        Logger::setLevel(Logger::LOG_DEBUG);
//        Logger::setLevel(Logger::LOG_INFORMATION);
        Logger::setLevel(Logger::LOG_ERROR);
    };
};

/**
 * Test to check that a sbml model can simulate using CVODEIntegrator
 * and not throw an error.
 *
 */
TEST_P(CVODEIntegratorIntegrationTests, CheckModelSimulates) {
    // get the sbml from the parameterised test (google "googlemock parameterised tests" for more info)
    SBMLTestModel *testModel = SBMLTestModelFactory(GetParam());
    // load model
    RoadRunner r(testModel->str());

    // do a simulation
    SimulateOptions options;
    options.start = 0.0;
    options.steps = 11;
    options.duration = 10;

    ASSERT_NO_THROW(
            auto result = r.simulate(&options);
            std::cout << *result << std::endl;
    );

    // The model has simulated 10 time points. So we dive into roadrunners
    // executable model and get the current state vector.
    int numStateVariables = r.getModel()->getNumFloatingSpecies();

    // allocate
    double *state = (double *) malloc(sizeof(double) * numStateVariables);

    // returns number of states copied, which may as well be used to clarify what we think we know
    int numCopied = r.getModel()->getStateVector(state);
    ASSERT_EQ(numCopied, numStateVariables);

    // get handle on the *known true values.
    auto trueValues = testModel->stateVectorAtT10();

    // we can now iterate over the collected states and compare them to the output of stateVectorAtT10.
    for (int i=0; i<numStateVariables; i++){
        std::string speciesName = r.getModel()->getStateVectorId(i);
        ASSERT_NEAR(trueValues[speciesName], state[i], 0.0001);
    }
    free(state);
    delete testModel;
}


/**
 * This test is similar to the `CheckModelSimulates` in that it checks
 * that the model integrates correctly. However, this time
 * we make a change in the model structure, regenerate the model
 * and then do the simulation
 */
//TEST_P(CVODEIntegratorIntegrationTests, CheckModelSimulatesAfterRegeneration) {
//    // get the sbml from the parameterised test (google "googlemock parameterised tests" for more info)
//    SBMLTestModel *testModel = SBMLTestModelFactory(GetParam());
//    // load model
//    RoadRunner r(testModel->str());
//
//    // do a simulation
//    SimulateOptions options;
//    options.start = 0.0;
//    options.steps = 11;
//    options.duration = 10;
//
//    ASSERT_NO_THROW(
//            auto result = r.simulate(&options);
//            std::cout << *result << std::endl;
//    );
//
//    //make a change to the model
//    r.addCompartment("DairyFarm", 1.0, false);
//    r.addSpecies("milk", "DairyFarm", 10.0, false, false, "", false);
//    r.addSpecies("cheese", "DairyFarm", 0.0, false, false, "", false);
//    r.addParameter("kCheese", 0.5, false);
//
//    // we now force model regeneration
//    r.addReaction("MilkToCheese" , {"milk"}, {"cheese"}, "kCheese*milk", true);
//
//    // another simulation
//    ASSERT_NO_THROW(
//            auto result = r.simulate(&options);
//            std::cout << *result << std::endl;
//    );
//
//    // The model has simulated 10 time points. So we dive into roadrunners
//    // executable model and get the current state vector.
//    int numStateVariables = r.getModel()->getNumFloatingSpecies();
//
//    // allocate
//    double *state = (double *) malloc(sizeof(double) * numStateVariables);
//
//    // returns number of states copied, which may as well be used to clarify what we think we know
//    int numCopied = r.getModel()->getStateVector(state);
//    ASSERT_EQ(numCopied, numStateVariables);
//
//    // get handle on the *known true values.
//    auto trueValues = testModel->stateVectorAtT10();
//
//    // we can now iterate over the collected states and compare them to the output of stateVectorAtT10.
//    for (int i=0; i<numStateVariables; i++){
//        std::string speciesName = r.getModel()->getStateVectorId(i);
//        ASSERT_NEAR(trueValues[speciesName], state[i], 0.0001);
//    }
//    free(state);
//    delete testModel;
//}

TEST(test, test){
    // find a way to have the integrator reset
    SBMLTestModel *testModel = SBMLTestModelFactory("Feedback");

    // load model
    RoadRunner r(testModel->str());


    // do a simulation
    SimulateOptions options;
    options.start = 0.0;
    options.steps = 11;
    options.duration = 10;

    r.simulate(&options);

    r.setBoundary("S1", true, true);

    r.simulate(&options);

    r.regenerate();
    delete testModel;
}


/**
 * This is where we actually supply the parameters to the
 * CVODEIntegratorIntegrationTests suite. All test fixtures
 * (those that start with TEST_P) will be run on all
 * models with the name provided here.
 *
 * @note If a particular model fails a particular test,
 * the best strategy is to temporarily comment out
 * all ::testing::Values to run the failing test in
 * isolation
 */
INSTANTIATE_TEST_SUITE_P(
        CheckModelSimulates, CVODEIntegratorIntegrationTests,
        ::testing::Values(
                "SimpleFlux",
                "Feedback"
        )
);


























