

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
        TestModel *testModelPtr = TestModelFactory(modelName);
        auto *testModel = dynamic_cast<ModelType *>(testModelPtr);

        std::cout << testModel->str() << std::endl;

        // load model
        RoadRunner r(testModel->str());
        r.getIntegrator()->setValue("stiff", false);

        // get handle on the *known true values.
        auto trueValues = testModel->timeSeriesResult();

        // grab integrator settings.
        const auto &settings = testModel->timeSeriesSettings();

        // apply settings
        SimulateOptions opt;
        for (const auto &setting: settings) {
            // opt.setItem(setting.first, setting.second); Surprisingly this strategy does not work. Suspected bug.
            // instead we'll have to manually unpack the simulation variables.
            if (setting.first == "reset_model") {
                opt.reset_model = setting.second;
            } else if (setting.first == "structured_result") {
                opt.structured_result = setting.second;
            } else if (setting.first == "copy_result") {
                opt.copy_result = setting.second;
            } else if (setting.first == "steps") {
                opt.steps = setting.second;
            } else if (setting.first == "start") {
                opt.start = setting.second;
            } else if (setting.first == "duration") {
                opt.duration = setting.second;
            }

            // then try applying integration level settings.
            try {
                r.getIntegrator()->setValue(setting.first, setting.second);
            } catch (std::exception &e) {
                // its okay to have a setting that doesn't belong here
                continue;
            }
        }

        const ls::DoubleMatrix *result = nullptr;

        ASSERT_NO_THROW(
                result = r.simulate(&opt);
                std::cout << *result << std::endl;
        );
        ASSERT_EQ(trueValues.numRows(), result->numRows());
        ASSERT_EQ(trueValues.numCols(), result->numCols());

        for (int i = 0; i < result->numRows(); i++) {
            for (int j = 0; j < result->numCols(); j++) {
                EXPECT_NEAR(trueValues[i][j], (*result)[i][j], 1e-3);
            }
        }
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

TEST_F(CVODEIntegratorIntegrationTests, TestFactorialInRateLaw) {
    CheckModelSimulates<FactorialInRateLaw>("FactorialInRateLaw");
}

TEST_F(CVODEIntegratorIntegrationTests, SimpleFlux){
    CheckModelSimulates<SimpleFlux>("SimpleFlux");
}

TEST_F(CVODEIntegratorIntegrationTests, OpenLinearFlux){
    CheckModelSimulates<OpenLinearFlux>("OpenLinearFlux");
}


























