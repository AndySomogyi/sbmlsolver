#include <rrRoadRunner.h>
#include "gtest/gtest.h"

#include "RoadRunnerTest.h"
#include "TestModelFactory.h"
#include "GillespieIntegrator.h"
#include "rrConfig.h"
#include "Matrix.h"

using namespace rr;

class JacobianTests : public RoadRunnerTest {

public:
    JacobianTests() = default;

    void checkJacobianFullAmt(const std::string& modelName, double tol){
        checkJac(modelName, tol, Config::ROADRUNNER_JACOBIAN_MODE_AMOUNTS, true);
    }

    void checkJacobianReducedAmt(const std::string& modelName, double tol){
        checkJac(modelName, tol, Config::ROADRUNNER_JACOBIAN_MODE_AMOUNTS, false);
    }

    void checkJacobianFullConc(const std::string& modelName, double tol){
        checkJac(modelName, tol, Config::ROADRUNNER_JACOBIAN_MODE_CONCENTRATIONS, true);
    }

    void checkJacobianReducedConc(const std::string& modelName, double tol){
        checkJac(modelName, tol, Config::ROADRUNNER_JACOBIAN_MODE_CONCENTRATIONS, false);
    }


private:
    /**
     * @brief check the values of TestModel jacobian interface against those computed by the current model
     * @param modelName A valid test model name (See TestModelFactory::getAvailableTestModelNames()
     * @param tol degree of tolerance, allowed error
     * @param rrJacMode either Config::ROADRUNNER_JACOBIAN_MODE_AMOUNTS or Config::ROADRUNNER_JACOBIAN_MODE_CONCENTRATIONS
     * @param full true if we are looking at full jacobian, false if were looking at reduced.
     * @details if full is set to false, then we turn on moiety conservation
     */
    static void checkJac(const std::string& modelName, double tol, Config::ROADRUNNER_JACOBIAN_MODE_VALUES rrJacMode, bool full){
        TestModel* testModel = TestModelFactory(modelName);
        JacobianResult* jacobianResultTestModel = dynamic_cast<JacobianResult*>(testModel);

        RoadRunner rr(testModel->str());

        Config::setValue(Config::ROADRUNNER_JACOBIAN_MODE, rrJacMode);

        ls::Matrix<double> expectedJac;

        if (full && rrJacMode == Config::ROADRUNNER_JACOBIAN_MODE_CONCENTRATIONS){
            expectedJac = jacobianResultTestModel->fullJacobianConc();
        } else if (full && rrJacMode == Config::ROADRUNNER_JACOBIAN_MODE_AMOUNTS){
            expectedJac = jacobianResultTestModel->fullJacobianAmt();
        } else if (!full && rrJacMode == Config::ROADRUNNER_JACOBIAN_MODE_CONCENTRATIONS){
            expectedJac = jacobianResultTestModel->reducedJacobianConc();
        } else if (!full && rrJacMode == Config::ROADRUNNER_JACOBIAN_MODE_AMOUNTS){
            expectedJac = jacobianResultTestModel->reducedJacobianAmt();
        } else {
            throw std::logic_error("Should never reach here. Check your input.");
        }

        ls::Matrix<double> actualJac;
        if (full){
            actualJac = rr.getFullJacobian();
        } else {
            rr.setConservedMoietyAnalysis(true);
            int numConservedMoieties = rr.getModel()->getNumConservedMoieties();
            if (numConservedMoieties == 0){
                rr.setConservedMoietyAnalysis(false);
            }
            actualJac = rr.getReducedJacobian();
        }

        // these matrix types have equality operators
        rr::Matrix<double> expectedJacMatrix(expectedJac);
        rr::Matrix<double> actualJacMatrix(actualJac);

        // true if these matrices are equal to tolerance tol
        bool passed = expectedJacMatrix.almostEquals(actualJacMatrix, tol);

        if (!passed){
            std::cout << "expectedJacMatrix" << std::endl;
            std::cout << expectedJacMatrix << std::endl;
            std::cout << "actualJacMatrix" << std::endl;
            std::cout <<  actualJacMatrix << std::endl;
        }
        ASSERT_TRUE(passed);
        delete testModel;
    }


};


TEST_F(JacobianTests, BiomolecularEndFullAmt) {
    checkJacobianFullAmt("BiomolecularEnd", 1e-4);
}

TEST_F(JacobianTests, BiomolecularEndFullConc) {
    checkJacobianFullConc("BiomolecularEnd", 1e-4);
}

TEST_F(JacobianTests, BiomolecularEndReducedAmt) {
    checkJacobianReducedAmt("BiomolecularEnd", 1e-4);
}

TEST_F(JacobianTests, BiomolecularEndReducedonc) {
    checkJacobianReducedConc("BiomolecularEnd", 1e-4);
}











