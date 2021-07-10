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
        double duration = settings["duration"];
        double steps = settings["steps"];
        double stepSize = (duration - start) / (steps);

        Matrix3D<double, double> sensResults(expectedResults.numRows(), trueValues.numCols());
        int numStates = model->getStateVector(nullptr);
        double *stateVector = new double[numStates];

        // collect initial state
        simulationResults(0, 0) = model->getTime();
        model->getStateVector(stateVector);
        for (int j = 1; j < numStates + 1; j++) {
            simulationResults(0, j) = stateVector[j - 1];
        }

        double t = 0.0;
        for (int i = 1; i < steps + 1; i++) {
            t = solver.integrate(t, stepSize);
            model->getStateVector(stateVector);
            simulationResults(i, 0) = model->getTime();
            for (int j = 1; j < numStates + 1; j++) {
                simulationResults(i, j) = stateVector[j - 1];
            }
        }

        bool passed = trueValues.almostEquals(simulationResults, 1e-4);

        if (!passed) {
            std::cout << "Expected result: " << std::endl;
            std::cout << trueValues << std::endl;
            std::cout << "Actual result: " << std::endl;
            std::cout << simulationResults << std::endl;
        }

        ASSERT_TRUE(passed);

        delete[] stateVector;
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
    std::cout << testModel.str() << std::endl;
    checkModelIntegrates<ForwardSensitivitySolver>(&testModel);
}

TEST_F(FFSIntegrationTests, getSensitivityMatrixWithFakeData) {
    RoadRunner r(OpenLinearFlux().str());
    ExecutableModel *model = r.getModel();
    ForwardSensitivitySolver forwardSensitivitySolver(model);
    forwardSensitivitySolver.integrate(0, 1);

    Matrix<double> results = forwardSensitivitySolver.getSensitivityMatrix();
    // this test doesn't care about accurate results since it only tests our
    // ability to retrieve sensitivity data. For accurate results testing
    // see the integrations tests.
    std::cout << results << std::endl;
}




















