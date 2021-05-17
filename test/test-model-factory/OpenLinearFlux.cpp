//
// Created by Ciaran on 17/05/2021.
//

#include "OpenLinearFlux.h"


std::string OpenLinearFlux::str() {
    return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
           "<sbml xmlns=\"http://www.sbml.org/sbml/level3/version1/core\" level=\"3\" version=\"1\">\n"
           "  <model metaid=\"x\" id=\"x\">\n"
           "    <listOfCompartments>\n"
           "      <compartment sboTerm=\"SBO:0000410\" id=\"default_compartment\" spatialDimensions=\"3\" size=\"1\" constant=\"true\"/>\n"
           "    </listOfCompartments>\n"
           "    <listOfSpecies>\n"
           "      <species id=\"S1\" compartment=\"default_compartment\" initialConcentration=\"0\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
           "      <species id=\"S2\" compartment=\"default_compartment\" initialConcentration=\"0\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
           "    </listOfSpecies>\n"
           "    <listOfParameters>\n"
           "      <parameter id=\"kin\" value=\"1\" constant=\"true\"/>\n"
           "      <parameter id=\"kf\" value=\"0.1\" constant=\"true\"/>\n"
           "      <parameter id=\"kout\" value=\"0.2\" constant=\"true\"/>\n"
           "      <parameter id=\"kb\" value=\"0.01\" constant=\"true\"/>\n"
           "    </listOfParameters>\n"
           "    <listOfReactions>\n"
           "      <reaction id=\"_J0\" reversible=\"false\" fast=\"false\">\n"
           "        <listOfProducts>\n"
           "          <speciesReference species=\"S1\" stoichiometry=\"1\" constant=\"true\"/>\n"
           "        </listOfProducts>\n"
           "        <kineticLaw>\n"
           "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
           "            <ci> kin </ci>\n"
           "          </math>\n"
           "        </kineticLaw>\n"
           "      </reaction>\n"
           "      <reaction id=\"_J1\" reversible=\"false\" fast=\"false\">\n"
           "        <listOfReactants>\n"
           "          <speciesReference species=\"S1\" stoichiometry=\"1\" constant=\"true\"/>\n"
           "        </listOfReactants>\n"
           "        <listOfProducts>\n"
           "          <speciesReference species=\"S2\" stoichiometry=\"1\" constant=\"true\"/>\n"
           "        </listOfProducts>\n"
           "        <kineticLaw>\n"
           "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
           "            <apply>\n"
           "              <times/>\n"
           "              <ci> kf </ci>\n"
           "              <ci> S1 </ci>\n"
           "            </apply>\n"
           "          </math>\n"
           "        </kineticLaw>\n"
           "      </reaction>\n"
           "      <reaction id=\"_J2\" reversible=\"false\" fast=\"false\">\n"
           "        <listOfReactants>\n"
           "          <speciesReference species=\"S2\" stoichiometry=\"1\" constant=\"true\"/>\n"
           "        </listOfReactants>\n"
           "        <kineticLaw>\n"
           "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
           "            <apply>\n"
           "              <times/>\n"
           "              <ci> S2 </ci>\n"
           "              <ci> kout </ci>\n"
           "            </apply>\n"
           "          </math>\n"
           "        </kineticLaw>\n"
           "      </reaction>\n"
           "    </listOfReactions>\n"
           "  </model>\n"
           "</sbml>";
}

StringDoubleMap OpenLinearFlux::steadyState() {
    return StringDoubleMap({
                                   {"S1", 10},
                                   {"S2", 5},
                           });
}

ls::DoubleMatrix OpenLinearFlux::timeSeriesResult() {
    return ls::DoubleMatrix({
                                    {0,  0,                  0},
                                    {1,  0.9516258196428962, 0.04527958486459608},
                                    {2,  1.812692469240729,  0.16429269841019906},
                                    {3,  2.5918177931655006, 0.33587597514094913},
                                    {4,  3.296799539619028,  0.5434443622065769},
                                    {5,  3.9346934028419236, 0.7740906110974121},
                                    {6,  4.51188363871537,   1.0178547271489167},
                                    {7,  5.034146961185131,  1.2671318458422862},
                                    {8,  5.506710357479626,  1.5161930250110387},
                                    {9,  5.934303400893279,  1.7607979250221717},
                                    {10, 6.321205586316753,  1.9978820848173928},
                            });
}

std::unordered_map<std::string, rr::Setting> OpenLinearFlux::timeSeriesSettings() {
    return std::unordered_map<std::string, rr::Setting>{
            {"start",    0},
            {"duration", 10},
            {"steps",    10},
    };
}

std::string OpenLinearFlux::modelName() {
    return "OpenLinearFlux";
}

std::unordered_map<std::string, rr::Setting> OpenLinearFlux::steadyStateSettings() {
    return std::unordered_map<std::string, rr::Setting>{
//            {"allow_presimulation", true},
            {"allow_presimulation",  false},
            {"presimulation_time",   5},
            {"moiety_conservation",  false},
            {"auto_moiety_analysis", false},
    };
}


ls::DoubleMatrix OpenLinearFlux::fullJacobianAmt() {
    return ls::DoubleMatrix(
            {
                    {-0.1, 0},
                    {0.1,  -0.2},
            });
}

ls::DoubleMatrix OpenLinearFlux::fullJacobianConc() {
    return ls::DoubleMatrix(
            {
                    {-0.1, 0},
                    {0.1,  -0.2},
            });
}

ls::DoubleMatrix OpenLinearFlux::reducedJacobianAmt() {
    return ls::DoubleMatrix(
            {
                    {-0.1, 0},
                    {0.1,  -0.2},
            });
}

ls::DoubleMatrix OpenLinearFlux::reducedJacobianConc() {
    return ls::DoubleMatrix(
            {
                    {-0.1, 0},
                    {0.1,  -0.2},
            });
}

std::unordered_map<std::string, rr::Setting> OpenLinearFlux::jacobianSettings() {
    return std::unordered_map<std::string, rr::Setting>();
}


