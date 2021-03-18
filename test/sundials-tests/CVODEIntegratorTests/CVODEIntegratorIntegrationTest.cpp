

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "rrRoadRunner.h"
#include "rrExecutableModel.h"
#include "CVODEIntegrator.h"
#include "Solver.h"
#include "TestModelFactory.h"

using namespace rr;
using namespace testing;


/**
 * Parameterised fixture for passing models through a series of tests.
 * The template parameter is a string and is used as input into TestModelFactory.
 */
class CVODEIntegratorIntegrationTests : public ::testing::Test {
public:

    CVODEIntegratorIntegrationTests() = default;

    /**
     * Test to check that a sbml model can simulate using CVODEIntegrator
     * and not throw an error.
     *
     */
    template<class ModelType>
    void CheckModelSimulates(const std::string &modelName) {
        SBMLTestModel* testModelPtr = TestModelFactory(modelName);
        ModelType* testModel = dynamic_cast<ModelType*>(testModelPtr);

        // load model
        RoadRunner r(testModel->str());
        r.getIntegrator()->setValue("stiff", false);

        // get handle on the *known true values.
        auto trueValues = testModel->stateVectorAtT10();

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
        auto *state = (double *) malloc(sizeof(double) * numStateVariables);

        // returns number of states copied, which may as well be used to clarify what we think we know
        int numCopied = r.getModel()->getStateVector(state);
        ASSERT_EQ(numCopied, numStateVariables);


        // we can now iterate over the collected states and compare them to the output of stateVectorAtT10.
        for (int i = 0; i < numStateVariables; i++) {
            std::string speciesName = r.getModel()->getStateVectorId(i);
            ASSERT_NEAR(trueValues[speciesName].second, state[i], 0.0001);
        }

        free(state);
        delete testModel;
    }
};

TEST_F(CVODEIntegratorIntegrationTests, TestSimpleFlux) {
    CheckModelSimulates<SimpleFlux>("SimpleFlux");
}

TEST_F(CVODEIntegratorIntegrationTests, TestModel269) {
    CheckModelSimulates<Model269>("Model269");
}

TEST_F(CVODEIntegratorIntegrationTests, TestModel28) {
    CheckModelSimulates<Model28>("Model28");
}

TEST_F(CVODEIntegratorIntegrationTests, TestCeilInRateLaw) {
    CheckModelSimulates<CeilInRateLaw>("CeilInRateLaw");
}

TEST_F(CVODEIntegratorIntegrationTests, TestFactorialInRateLaw) {
    CheckModelSimulates<FactorialInRateLaw>("FactorialInRateLaw");
}


























