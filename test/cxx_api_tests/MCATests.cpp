//
// Created by Ciaran on 21/07/2021.
//

#include <rrRoadRunner.h>
#include "gtest/gtest.h"

#include "RoadRunnerTest.h"
#include "TestModelFactory.h"
#include "GillespieIntegrator.h"
#include "rrConfig.h"
#include "Matrix.h"

using namespace rr;

class MCATests : public RoadRunnerTest {

public:
    MCATests() = default;

    void checkUnscaledConcControlMatrix(const std::string& modelName, double tol){
        TestModel* testModel = TestModelFactory(modelName);
        MCAResult* mcaTestModel = dynamic_cast<MCAResult*>(testModel);
        assert(mcaTestModel && "Test model probably does not implement the MCAResult interface");
        RoadRunner rr(testModel->str());
        ls::Matrix<double> actual = rr.getUnscaledConcentrationControlCoefficientMatrix();
        rr::Matrix<double> expected = mcaTestModel->unscaledConcentrationControlCoefficientMatrix();
        std::cout << "expected" << std::endl;
        std::cout << expected << std::endl;
        std::cout << "actual" << std::endl;
        std::cout << actual << std::endl;
        checkMatrixEqual(expected, actual, tol);
    }

    void checkScaledConcControlMatrix(const std::string& modelName, double tol){
        TestModel* testModel = TestModelFactory(modelName);
        MCAResult* mcaTestModel = dynamic_cast<MCAResult*>(testModel);
        assert(mcaTestModel && "Test model probably does not implement the MCAResult interface");
        RoadRunner rr(testModel->str());
        ls::Matrix<double> actual = rr.getScaledConcentrationControlCoefficientMatrix();
        rr::Matrix<double> expected = mcaTestModel->scaledConcentrationControlCoefficientMatrix();
        std::cout << "expected" << std::endl;
        std::cout << expected << std::endl;
        std::cout << "actual" << std::endl;
        std::cout << actual << std::endl;

        checkMatrixEqual(expected, actual, tol);
    }

    void checkUnscaledFluxControlMatrix(const std::string& modelName, double tol){
        TestModel* testModel = TestModelFactory(modelName);
        MCAResult* mcaTestModel = dynamic_cast<MCAResult*>(testModel);
        assert(mcaTestModel && "Test model probably does not implement the MCAResult interface");
        RoadRunner rr(testModel->str());
        ls::Matrix<double> actual = rr.getUnscaledFluxControlCoefficientMatrix();
        rr::Matrix<double> expected = mcaTestModel->unscaledFluxControlCoefficientMatrix();
        std::cout << "expected" << std::endl;
        std::cout << expected << std::endl;
        std::cout << "actual" << std::endl;
        std::cout << actual << std::endl;
        checkMatrixEqual(expected, actual, tol);
    }

    void checkScaledFluxControlMatrix(const std::string& modelName, double tol){
        TestModel* testModel = TestModelFactory(modelName);
        MCAResult* mcaTestModel = dynamic_cast<MCAResult*>(testModel);
        assert(mcaTestModel && "Test model probably does not implement the MCAResult interface");
        RoadRunner rr(testModel->str());
        ls::Matrix<double> actual = rr.getScaledFluxControlCoefficientMatrix();
        rr::Matrix<double> expected = mcaTestModel->scaledFluxControlCoefficientMatrix();
        std::cout << "expected" << std::endl;
        std::cout << expected << std::endl;
        std::cout << "actual" << std::endl;
        std::cout << actual << std::endl;
        checkMatrixEqual(expected, actual, tol);
    }
};
// unscaled conc
TEST_F(MCATests, BiomolecularEndUnscaledConcControlMatrix){
    checkUnscaledConcControlMatrix("BiomolecularEnd", 1e-3);
}

// scaled conc
TEST_F(MCATests, BiomolecularEndScaledConcControlMatrix){
    checkScaledConcControlMatrix("BiomolecularEnd", 1e-3);
}

// unscaled flux
TEST_F(MCATests, BiomolecularEndUnscaledFluxControlMatrix){
    checkUnscaledFluxControlMatrix("BiomolecularEnd",  1e-3);
}

// scaled flux
TEST_F(MCATests, BiomolecularEndScaledFluxControlMatrix){
    checkScaledFluxControlMatrix("BiomolecularEnd",  1e-3);
}

























