//
// Created by Ciaran Welsh on 09/06/2021.
//

#include "gtest/gtest.h"
#include "rrRoadRunner.h"
#include "TestModelFactory.h"
#include "ForwardSensitivitySolver.h"
#include "ForwardSensitivitySolver.h"
#include "../CVODEIntegratorTests/CvodeIntegrationTest.h"

using namespace rr;

class FFSIntegrationTests : public CVODEIntegratorTests {
public:
    FFSIntegrationTests() = default;

    /**
     * @brief Integrate the model and compute sensitivities. Compare
     * results against expected values
     */
    template<class SolverType = ForwardSensitivitySolver>
    void checkTimeSeriesSensitivities(TestModel *testModel) {

        // load model
        RoadRunner r(testModel->str());

        auto *timeSeriesSensResult = dynamic_cast<TimeSeriesSensitivityResult *>(testModel);

        ExecutableModel *model = r.getModel();

        SolverType solver(model);
        solver.setValue("stiff", false);

        // get handle on the *known true values.
        auto expectedResults = timeSeriesSensResult->timeSeriesSensitivityResult();

        // grab integrator settings.
        auto settings = timeSeriesSensResult->timeSeriesSensitivityResultSettings();

        double start = settings["start"];
        double stop = settings["duration"];
        int    steps = settings["steps"];

        std::cout << "start: " << start << ": stop: " << stop << "; steps: "<< steps << std::endl;

        Matrix3D<double, double> actualResults = solver.simulate(start, stop, steps);

        bool passed = expectedResults.almostEquals(actualResults, 1e-4);

        if (!passed) {
            std::cout << "Expected result: " << std::endl;
            std::cout << expectedResults << std::endl;
            std::cout << "Actual result: " << std::endl;
            std::cout << actualResults << std::endl;
        }

        ASSERT_TRUE(passed);
    }
};


TEST_F(FFSIntegrationTests, CheckTimeSeriesAccurateTestSimpleFlux) {
    SimpleFlux testModel;
    checkModelIntegrates<ForwardSensitivitySolver>(&testModel);
}

TEST_F(FFSIntegrationTests, CheckTimeSeriesAccurateOpenLinearFlux) {
    OpenLinearFlux testModel;
    checkModelIntegrates<ForwardSensitivitySolver>(&testModel);
}

TEST_F(FFSIntegrationTests, CheckTimeSeriesAccurateTestModel269) {
    Model269 testModel;
    checkModelIntegrates<ForwardSensitivitySolver>(&testModel);
}

TEST_F(FFSIntegrationTests, CheckTimeSeriesAccurateTestModel28) {
    Model28 testModel;
    checkModelIntegrates<ForwardSensitivitySolver>(&testModel);
}

TEST_F(FFSIntegrationTests, CheckTimeSeriesAccurateFactorialInRateLaw) {
    FactorialInRateLaw testModel;
    checkModelIntegrates<ForwardSensitivitySolver>(&testModel);
}




TEST_F(FFSIntegrationTests, CheckTimeSeriesSensAccurateTestSimpleFlux) {
    SimpleFlux testModel;
    checkTimeSeriesSensitivities<ForwardSensitivitySolver>(&testModel);
}


TEST_F(FFSIntegrationTests, CheckTimeSeriesSensAccurateTestModel269) {
    Model269 testModel;
    checkTimeSeriesSensitivities<ForwardSensitivitySolver>(&testModel);
}

TEST_F(FFSIntegrationTests, CheckTimeSeriesSensAccurateTestModel28) {
    Model28 testModel;
    checkTimeSeriesSensitivities<ForwardSensitivitySolver>(&testModel);
}

TEST_F(FFSIntegrationTests, CheckTimeSeriesSensAccurateFactorialInRateLaw) {
    FactorialInRateLaw testModel;
    checkTimeSeriesSensitivities<ForwardSensitivitySolver>(&testModel);
}

TEST_F(FFSIntegrationTests, CheckTimeSeriesSensAccurateOpenLinearFlux) {
    OpenLinearFlux testModel;
    checkTimeSeriesSensitivities<ForwardSensitivitySolver>(&testModel);
}

TEST_F(FFSIntegrationTests, CheckRoadRunnerInterfaceWorks) {
    RoadRunner rr(SimpleFlux().str());
    std::cout << rr.timeSeriesSensitivities(0, 10, 11) << std::endl;
}




















