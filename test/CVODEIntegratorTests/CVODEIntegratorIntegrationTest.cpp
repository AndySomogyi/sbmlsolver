

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
 * The template parameter is a string and is used as input into SBMLTestModelFactory.
 */
class CVODEIntegratorIntegrationTests : public ::testing::TestWithParam<std::string> {
public:

    CVODEIntegratorIntegrationTests() {
//        Logger::setLevel(Logger::LOG_DEBUG);
//        Logger::setLevel(Logger::LOG_INFORMATION);
//        Logger::setLevel(Logger::LOG_ERROR);
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
    r.getIntegrator()->setValue("stiff", true);

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
 * This is where we actually supply the parameters to the
 * CVODEIntegratorIntegrationTests suite. All tests
 * (start with TEST_P) will be run on all
 * models with the name provided here.
 *
 * @note If a particular model fails a particular test,
 * the best strategy is to temporarily comment out
 * all other ::testing::Values to run the failing test in
 * isolation
 */
INSTANTIATE_TEST_SUITE_P(
        CheckModelSimulates, CVODEIntegratorIntegrationTests,
        ::testing::Values(
//                "SimpleFlux",
//                "Feedback",
//                "Model269",
                "Model28"
//                "CeilInRateLaw",
//                "FactorialInRateLaw"
        )
);


























