
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "rrRoadRunner.h"
#include "rrExecutableModel.h"
#include "CVODEIntegrator.h"
#include "Solver.h"
#include "TestModelFactory.h"

using namespace rr;

class CVODEIntegratorUnitTests : public ::testing::Test {
public:

    CVODEIntegratorUnitTests() = default;

};


TEST_F(CVODEIntegratorUnitTests, SetConcentrationToleranceFromScalar) {
    RoadRunner rr(TestModelFactory("OpenLinearFlux")->str());
    Integrator *cvodeIntegrator = rr.getIntegrator();
    cvodeIntegrator->setConcentrationTolerance(0.1234);
    auto x = cvodeIntegrator->getConcentrationTolerance();
    for (auto i: x) {
        ASSERT_NEAR(i, 0.1234, 1e-7);
    }
}

TEST_F(CVODEIntegratorUnitTests, SetConcentrationToleranceFromScalarSmallValue) {
    RoadRunner rr(TestModelFactory("OpenLinearFlux")->str());
    Integrator *cvodeIntegrator = rr.getIntegrator();
    cvodeIntegrator->setConcentrationTolerance(1e-15);
    auto x = cvodeIntegrator->getConcentrationTolerance();
    for (auto i: x) {
        ASSERT_NEAR(i, 1e-15, 1e-7);
    }
}

TEST_F(CVODEIntegratorUnitTests, SetConcentrationToleranceFromVector) {
    RoadRunner rr(TestModelFactory("OpenLinearFlux")->str());
    Integrator *cvodeIntegrator = rr.getIntegrator();
    std::vector<double> d({0.5678, 0.4321});
    cvodeIntegrator->setConcentrationTolerance(d);
    ASSERT_EQ(d, cvodeIntegrator->getConcentrationTolerance());
}

TEST_F(CVODEIntegratorUnitTests, t) {
    std::string sbml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                       "<sbml xmlns=\"http://www.sbml.org/sbml/level3/version1/core\" level=\"3\" version=\"1\">\n"
                       "  <model metaid=\"__main\" id=\"__main\">\n"
                       "    <listOfCompartments>\n"
                       "      <compartment id=\"c1\" spatialDimensions=\"3\" size=\"1\" constant=\"true\"/>\n"
                       "    </listOfCompartments>\n"
                       "    <listOfSpecies>\n"
                       "      <species id=\"S0\" compartment=\"c1\" initialConcentration=\"1\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
                       "      <species id=\"S1\" compartment=\"c1\" initialConcentration=\"5\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
                       "      <species id=\"S2\" compartment=\"c1\" initialConcentration=\"9\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
                       "      <species id=\"S3\" compartment=\"c1\" initialConcentration=\"3\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
                       "      <species id=\"S4\" compartment=\"c1\" initialConcentration=\"10\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
                       "      <species id=\"S5\" compartment=\"c1\" initialConcentration=\"3\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
                       "      <species id=\"S6\" compartment=\"c1\" initialConcentration=\"7\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"true\" constant=\"false\"/>\n"
                       "      <species id=\"S7\" compartment=\"c1\" initialConcentration=\"1\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"true\" constant=\"false\"/>\n"
                       "      <species id=\"S8\" compartment=\"c1\" initialConcentration=\"6\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"true\" constant=\"false\"/>\n"
                       "      <species id=\"S9\" compartment=\"c1\" initialConcentration=\"3\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"true\" constant=\"false\"/>\n"
                       "    </listOfSpecies>\n"
                       "    <listOfParameters>\n"
                       "      <parameter id=\"k0\" value=\"2.60142584927558\" constant=\"true\"/>\n"
                       "      <parameter id=\"k1\" value=\"25.3866965166561\" constant=\"true\"/>\n"
                       "      <parameter id=\"k2\" value=\"25.9671539901853\" constant=\"true\"/>\n"
                       "      <parameter id=\"k3\" value=\"3.34302175839196\" constant=\"true\"/>\n"
                       "      <parameter id=\"k4\" value=\"42.1099522080306\" constant=\"true\"/>\n"
                       "      <parameter id=\"k5\" value=\"2.19863788015643\" constant=\"true\"/>\n"
                       "      <parameter id=\"k6\" value=\"12.2019138115565\" constant=\"true\"/>\n"
                       "      <parameter id=\"k7\" value=\"17.3490404499765\" constant=\"true\"/>\n"
                       "      <parameter id=\"k8\" value=\"45.2057617455586\" constant=\"true\"/>\n"
                       "      <parameter id=\"k9\" value=\"38.7920704394073\" constant=\"true\"/>\n"
                       "      <parameter id=\"k10\" value=\"43.5202554107622\" constant=\"true\"/>\n"
                       "      <parameter id=\"k11\" value=\"10.7813407470585\" constant=\"true\"/>\n"
                       "    </listOfParameters>\n"
                       "    <listOfReactions>\n"
                       "      <reaction id=\"r0\" reversible=\"false\" fast=\"false\">\n"
                       "        <listOfReactants>\n"
                       "          <speciesReference species=\"S8\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfReactants>\n"
                       "        <listOfProducts>\n"
                       "          <speciesReference species=\"S3\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfProducts>\n"
                       "        <kineticLaw>\n"
                       "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
                       "            <apply>\n"
                       "              <times/>\n"
                       "              <ci> k0 </ci>\n"
                       "              <ci> S8 </ci>\n"
                       "            </apply>\n"
                       "          </math>\n"
                       "        </kineticLaw>\n"
                       "      </reaction>\n"
                       "      <reaction id=\"r1\" reversible=\"false\" fast=\"false\">\n"
                       "        <listOfReactants>\n"
                       "          <speciesReference species=\"S2\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfReactants>\n"
                       "        <listOfProducts>\n"
                       "          <speciesReference species=\"S1\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "          <speciesReference species=\"S2\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfProducts>\n"
                       "        <kineticLaw>\n"
                       "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
                       "            <apply>\n"
                       "              <times/>\n"
                       "              <ci> k1 </ci>\n"
                       "              <ci> S2 </ci>\n"
                       "            </apply>\n"
                       "          </math>\n"
                       "        </kineticLaw>\n"
                       "      </reaction>\n"
                       "      <reaction id=\"r2\" reversible=\"false\" fast=\"false\">\n"
                       "        <listOfReactants>\n"
                       "          <speciesReference species=\"S4\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfReactants>\n"
                       "        <listOfProducts>\n"
                       "          <speciesReference species=\"S9\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "          <speciesReference species=\"S9\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfProducts>\n"
                       "        <kineticLaw>\n"
                       "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
                       "            <apply>\n"
                       "              <times/>\n"
                       "              <ci> k2 </ci>\n"
                       "              <ci> S4 </ci>\n"
                       "            </apply>\n"
                       "          </math>\n"
                       "        </kineticLaw>\n"
                       "      </reaction>\n"
                       "      <reaction id=\"r3\" reversible=\"false\" fast=\"false\">\n"
                       "        <listOfReactants>\n"
                       "          <speciesReference species=\"S8\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "          <speciesReference species=\"S8\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfReactants>\n"
                       "        <listOfProducts>\n"
                       "          <speciesReference species=\"S5\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfProducts>\n"
                       "        <listOfModifiers>\n"
                       "          <modifierSpeciesReference species=\"S2\"/>\n"
                       "        </listOfModifiers>\n"
                       "        <kineticLaw>\n"
                       "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
                       "            <apply>\n"
                       "              <times/>\n"
                       "              <ci> k3 </ci>\n"
                       "              <ci> S8 </ci>\n"
                       "              <ci> S2 </ci>\n"
                       "            </apply>\n"
                       "          </math>\n"
                       "        </kineticLaw>\n"
                       "      </reaction>\n"
                       "      <reaction id=\"r4\" reversible=\"false\" fast=\"false\">\n"
                       "        <listOfReactants>\n"
                       "          <speciesReference species=\"S7\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "          <speciesReference species=\"S7\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfReactants>\n"
                       "        <listOfProducts>\n"
                       "          <speciesReference species=\"S2\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfProducts>\n"
                       "        <listOfModifiers>\n"
                       "          <modifierSpeciesReference species=\"S1\"/>\n"
                       "        </listOfModifiers>\n"
                       "        <kineticLaw>\n"
                       "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
                       "            <apply>\n"
                       "              <times/>\n"
                       "              <ci> k4 </ci>\n"
                       "              <ci> S7 </ci>\n"
                       "              <ci> S1 </ci>\n"
                       "            </apply>\n"
                       "          </math>\n"
                       "        </kineticLaw>\n"
                       "      </reaction>\n"
                       "      <reaction id=\"r5\" reversible=\"false\" fast=\"false\">\n"
                       "        <listOfReactants>\n"
                       "          <speciesReference species=\"S7\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "          <speciesReference species=\"S7\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfReactants>\n"
                       "        <listOfProducts>\n"
                       "          <speciesReference species=\"S4\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfProducts>\n"
                       "        <kineticLaw>\n"
                       "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
                       "            <apply>\n"
                       "              <times/>\n"
                       "              <ci> k5 </ci>\n"
                       "              <ci> S7 </ci>\n"
                       "              <ci> S7 </ci>\n"
                       "            </apply>\n"
                       "          </math>\n"
                       "        </kineticLaw>\n"
                       "      </reaction>\n"
                       "      <reaction id=\"r6\" reversible=\"false\" fast=\"false\">\n"
                       "        <listOfReactants>\n"
                       "          <speciesReference species=\"S0\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfReactants>\n"
                       "        <listOfProducts>\n"
                       "          <speciesReference species=\"S9\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfProducts>\n"
                       "        <kineticLaw>\n"
                       "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
                       "            <apply>\n"
                       "              <times/>\n"
                       "              <ci> k6 </ci>\n"
                       "              <ci> S0 </ci>\n"
                       "            </apply>\n"
                       "          </math>\n"
                       "        </kineticLaw>\n"
                       "      </reaction>\n"
                       "      <reaction id=\"r7\" reversible=\"false\" fast=\"false\">\n"
                       "        <listOfReactants>\n"
                       "          <speciesReference species=\"S5\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "          <speciesReference species=\"S5\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfReactants>\n"
                       "        <listOfProducts>\n"
                       "          <speciesReference species=\"S3\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfProducts>\n"
                       "        <kineticLaw>\n"
                       "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
                       "            <apply>\n"
                       "              <times/>\n"
                       "              <ci> k7 </ci>\n"
                       "              <ci> S5 </ci>\n"
                       "              <ci> S5 </ci>\n"
                       "            </apply>\n"
                       "          </math>\n"
                       "        </kineticLaw>\n"
                       "      </reaction>\n"
                       "      <reaction id=\"r8\" reversible=\"false\" fast=\"false\">\n"
                       "        <listOfReactants>\n"
                       "          <speciesReference species=\"S3\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "          <speciesReference species=\"S3\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfReactants>\n"
                       "        <listOfProducts>\n"
                       "          <speciesReference species=\"S1\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfProducts>\n"
                       "        <kineticLaw>\n"
                       "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
                       "            <apply>\n"
                       "              <times/>\n"
                       "              <ci> k8 </ci>\n"
                       "              <ci> S3 </ci>\n"
                       "              <ci> S1 </ci>\n"
                       "            </apply>\n"
                       "          </math>\n"
                       "        </kineticLaw>\n"
                       "      </reaction>\n"
                       "      <reaction id=\"r9\" reversible=\"false\" fast=\"false\">\n"
                       "        <listOfReactants>\n"
                       "          <speciesReference species=\"S2\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfReactants>\n"
                       "        <listOfProducts>\n"
                       "          <speciesReference species=\"S5\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "          <speciesReference species=\"S9\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfProducts>\n"
                       "        <kineticLaw>\n"
                       "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
                       "            <apply>\n"
                       "              <times/>\n"
                       "              <ci> k9 </ci>\n"
                       "              <ci> S2 </ci>\n"
                       "            </apply>\n"
                       "          </math>\n"
                       "        </kineticLaw>\n"
                       "      </reaction>\n"
                       "      <reaction id=\"r10\" reversible=\"false\" fast=\"false\">\n"
                       "        <listOfReactants>\n"
                       "          <speciesReference species=\"S4\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "          <speciesReference species=\"S4\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfReactants>\n"
                       "        <listOfProducts>\n"
                       "          <speciesReference species=\"S9\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfProducts>\n"
                       "        <kineticLaw>\n"
                       "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
                       "            <apply>\n"
                       "              <times/>\n"
                       "              <ci> k10 </ci>\n"
                       "              <ci> S4 </ci>\n"
                       "              <ci> S9 </ci>\n"
                       "            </apply>\n"
                       "          </math>\n"
                       "        </kineticLaw>\n"
                       "      </reaction>\n"
                       "      <reaction id=\"r11\" reversible=\"false\" fast=\"false\">\n"
                       "        <listOfReactants>\n"
                       "          <speciesReference species=\"S6\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "          <speciesReference species=\"S6\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfReactants>\n"
                       "        <listOfProducts>\n"
                       "          <speciesReference species=\"S0\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfProducts>\n"
                       "        <listOfModifiers>\n"
                       "          <modifierSpeciesReference species=\"S4\"/>\n"
                       "        </listOfModifiers>\n"
                       "        <kineticLaw>\n"
                       "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
                       "            <apply>\n"
                       "              <times/>\n"
                       "              <ci> k11 </ci>\n"
                       "              <ci> S6 </ci>\n"
                       "              <ci> S4 </ci>\n"
                       "            </apply>\n"
                       "          </math>\n"
                       "        </kineticLaw>\n"
                       "      </reaction>\n"
                       "    </listOfReactions>\n"
                       "  </model>\n"
                       "</sbml>";
    RoadRunner r(sbml);
    SimulateOptions options;
    options.start = 0;
    options.steps = 5;
    options.duration = 100;
    auto res = r.simulate(&options);
    std::cout << res << std::endl;


}


















