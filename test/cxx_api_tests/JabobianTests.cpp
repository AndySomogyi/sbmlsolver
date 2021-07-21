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


TEST_F(JacobianTests, BimolecularEndFullAmt) {
    checkJacobianFullAmt("BimolecularEnd", 1e-4);
}

TEST_F(JacobianTests, BimolecularEndFullConc) {
    checkJacobianFullConc("BimolecularEnd", 1e-4);
}

TEST_F(JacobianTests, BimolecularEndReducedAmt) {
    checkJacobianReducedAmt("BimolecularEnd", 1e-4);
}

TEST_F(JacobianTests, BimolecularEndReducedonc) {
    checkJacobianReducedConc("BimolecularEnd", 1e-4);
}

TEST_F(JacobianTests, rrate) {
    std::string sbml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                       "<!-- Created by libAntimony version v2.5.2 on 2014-09-22 11:05 with libSBML version 5.10.2. -->\n"
                       "<sbml xmlns=\"http://www.sbml.org/sbml/level3/version1/core\" level=\"3\" version=\"1\">\n"
                       "  <model id=\"Bimolecular_end\" name=\"Bimolecular_end\">\n"
                       "    <listOfFunctionDefinitions>\n"
                       "      <functionDefinition id=\"MM\">\n"
                       "        <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
                       "          <lambda>\n"
                       "            <bvar>\n"
                       "              <ci> S1 </ci>\n"
                       "            </bvar>\n"
                       "            <bvar>\n"
                       "              <ci> S2 </ci>\n"
                       "            </bvar>\n"
                       "            <bvar>\n"
                       "              <ci> Vm </ci>\n"
                       "            </bvar>\n"
                       "            <bvar>\n"
                       "              <ci> Km1 </ci>\n"
                       "            </bvar>\n"
                       "            <bvar>\n"
                       "              <ci> Km2 </ci>\n"
                       "            </bvar>\n"
                       "            <bvar>\n"
                       "              <ci> Keq </ci>\n"
                       "            </bvar>\n"
                       "            <apply>\n"
                       "              <divide/>\n"
                       "              <apply>\n"
                       "                <times/>\n"
                       "                <apply>\n"
                       "                  <divide/>\n"
                       "                  <ci> Vm </ci>\n"
                       "                  <ci> Km1 </ci>\n"
                       "                </apply>\n"
                       "                <apply>\n"
                       "                  <minus/>\n"
                       "                  <ci> S1 </ci>\n"
                       "                  <apply>\n"
                       "                    <divide/>\n"
                       "                    <ci> S2 </ci>\n"
                       "                    <ci> Keq </ci>\n"
                       "                  </apply>\n"
                       "                </apply>\n"
                       "              </apply>\n"
                       "              <apply>\n"
                       "                <plus/>\n"
                       "                <cn type=\"integer\"> 1 </cn>\n"
                       "                <apply>\n"
                       "                  <divide/>\n"
                       "                  <ci> S1 </ci>\n"
                       "                  <ci> Km1 </ci>\n"
                       "                </apply>\n"
                       "                <apply>\n"
                       "                  <divide/>\n"
                       "                  <ci> S2 </ci>\n"
                       "                  <ci> Km2 </ci>\n"
                       "                </apply>\n"
                       "              </apply>\n"
                       "            </apply>\n"
                       "          </lambda>\n"
                       "        </math>\n"
                       "      </functionDefinition>\n"
                       "    </listOfFunctionDefinitions>\n"
                       "    <listOfCompartments>\n"
                       "      <compartment sboTerm=\"SBO:0000410\" id=\"default_compartment\" spatialDimensions=\"3\" size=\"1\" constant=\"true\"/>\n"
                       "    </listOfCompartments>\n"
                       "    <listOfSpecies>\n"
                       "      <species id=\"X0\" compartment=\"default_compartment\" initialConcentration=\"8.03\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"true\" constant=\"false\"/>\n"
                       "      <species id=\"S1\" compartment=\"default_compartment\" initialConcentration=\"7.12\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
                       "      <species id=\"S2\" compartment=\"default_compartment\" initialConcentration=\"3.97\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
                       "      <species id=\"S3\" compartment=\"default_compartment\" initialConcentration=\"0.96\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
                       "      <species id=\"X1\" compartment=\"default_compartment\" initialConcentration=\"0.54\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"true\" constant=\"false\"/>\n"
                       "    </listOfSpecies>\n"
                       "    <listOfReactions>\n"
                       "      <reaction id=\"J0\" reversible=\"true\" fast=\"false\">\n"
                       "        <listOfReactants>\n"
                       "          <speciesReference species=\"X0\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfReactants>\n"
                       "        <listOfProducts>\n"
                       "          <speciesReference species=\"S1\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfProducts>\n"
                       "        <kineticLaw>\n"
                       "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
                       "            <apply>\n"
                       "              <ci> MM </ci>\n"
                       "              <ci> X0 </ci>\n"
                       "              <ci> S1 </ci>\n"
                       "              <cn> 4.39 </cn>\n"
                       "              <cn> 9.85 </cn>\n"
                       "              <cn> 7.38 </cn>\n"
                       "              <cn> 6.12 </cn>\n"
                       "            </apply>\n"
                       "          </math>\n"
                       "        </kineticLaw>\n"
                       "      </reaction>\n"
                       "      <reaction id=\"J1\" reversible=\"true\" fast=\"false\">\n"
                       "        <listOfReactants>\n"
                       "          <speciesReference species=\"S1\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfReactants>\n"
                       "        <listOfProducts>\n"
                       "          <speciesReference species=\"S2\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfProducts>\n"
                       "        <kineticLaw>\n"
                       "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
                       "            <apply>\n"
                       "              <ci> MM </ci>\n"
                       "              <ci> S1 </ci>\n"
                       "              <ci> S2 </ci>\n"
                       "              <cn> 6.12 </cn>\n"
                       "              <cn> 9.15 </cn>\n"
                       "              <cn> 1.59 </cn>\n"
                       "              <cn> 4.68 </cn>\n"
                       "            </apply>\n"
                       "          </math>\n"
                       "        </kineticLaw>\n"
                       "      </reaction>\n"
                       "      <reaction id=\"J2\" reversible=\"true\" fast=\"false\">\n"
                       "        <listOfReactants>\n"
                       "          <speciesReference species=\"S2\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfReactants>\n"
                       "        <listOfProducts>\n"
                       "          <speciesReference species=\"S3\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfProducts>\n"
                       "        <kineticLaw>\n"
                       "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
                       "            <apply>\n"
                       "              <ci> MM </ci>\n"
                       "              <ci> S2 </ci>\n"
                       "              <ci> S3 </ci>\n"
                       "              <cn> 4.68 </cn>\n"
                       "              <cn> 8.22 </cn>\n"
                       "              <cn> 4.29 </cn>\n"
                       "              <cn> 0.57 </cn>\n"
                       "            </apply>\n"
                       "          </math>\n"
                       "        </kineticLaw>\n"
                       "      </reaction>\n"
                       "      <reaction id=\"J3\" reversible=\"true\" fast=\"false\">\n"
                       "        <listOfReactants>\n"
                       "          <speciesReference species=\"S3\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfReactants>\n"
                       "        <listOfProducts>\n"
                       "          <speciesReference species=\"X1\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfProducts>\n"
                       "        <kineticLaw>\n"
                       "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
                       "            <apply>\n"
                       "              <ci> MM </ci>\n"
                       "              <ci> S3 </ci>\n"
                       "              <ci> X1 </ci>\n"
                       "              <cn> 0.57 </cn>\n"
                       "              <cn> 0.8 </cn>\n"
                       "              <cn> 2.2 </cn>\n"
                       "              <cn> 4.65 </cn>\n"
                       "            </apply>\n"
                       "          </math>\n"
                       "        </kineticLaw>\n"
                       "      </reaction>\n"
                       "      <reaction id=\"J4\" reversible=\"true\" fast=\"false\">\n"
                       "        <listOfReactants>\n"
                       "          <speciesReference species=\"S2\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "          <speciesReference species=\"S3\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfReactants>\n"
                       "        <listOfProducts>\n"
                       "          <speciesReference species=\"S1\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfProducts>\n"
                       "        <kineticLaw>\n"
                       "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
                       "            <apply>\n"
                       "              <minus/>\n"
                       "              <apply>\n"
                       "                <times/>\n"
                       "                <cn> 4.65 </cn>\n"
                       "                <ci> S2 </ci>\n"
                       "                <ci> S3 </ci>\n"
                       "              </apply>\n"
                       "              <apply>\n"
                       "                <times/>\n"
                       "                <cn> 7.61 </cn>\n"
                       "                <ci> S1 </ci>\n"
                       "              </apply>\n"
                       "            </apply>\n"
                       "          </math>\n"
                       "        </kineticLaw>\n"
                       "      </reaction>\n"
                       "    </listOfReactions>\n"
                       "  </model>\n"
                       "</sbml>";
    RoadRunner rr(sbml);

    std::vector<int> idx(5);
    for (int i=0; i<5; i++){
        idx[i] = i;
    }
    std::vector<double> val(5);
    auto x = rr.getModel()->getReactionRates(5, idx.data(), val.data());

    for (int i=0; i<5; i++){
        std::cout << val[i] << std::endl;
    }
}











