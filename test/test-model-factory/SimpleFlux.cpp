//
// Created by Ciaran on 17/05/2021.
//

#include "SimpleFlux.h"

std::string SimpleFlux::str() {
    return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
           "<sbml xmlns=\"http://www.sbml.org/sbml/level3/version1/core\" level=\"3\" version=\"1\">\n"
           "  <model metaid=\"x\" id=\"x\">\n"
           "    <listOfCompartments>\n"
           "      <compartment sboTerm=\"SBO:0000410\" id=\"default_compartment\" spatialDimensions=\"3\" size=\"1\" constant=\"true\"/>\n"
           "    </listOfCompartments>\n"
           "    <listOfSpecies>\n"
           "      <species id=\"S1\" compartment=\"default_compartment\" initialConcentration=\"10\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
           "      <species id=\"S2\" compartment=\"default_compartment\" initialConcentration=\"1\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
           "    </listOfSpecies>\n"
           "    <listOfParameters>\n"
           "      <parameter id=\"kf\" value=\"0.1\" constant=\"true\"/>\n"
           "      <parameter id=\"kb\" value=\"0.01\" constant=\"true\"/>\n"
           "    </listOfParameters>\n"
           "    <listOfReactions>\n"
           "      <reaction id=\"_J0\" reversible=\"false\" fast=\"false\">\n"
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
           "      <reaction id=\"_J1\" reversible=\"false\" fast=\"false\">\n"
           "        <listOfReactants>\n"
           "          <speciesReference species=\"S2\" stoichiometry=\"1\" constant=\"true\"/>\n"
           "        </listOfReactants>\n"
           "        <listOfProducts>\n"
           "          <speciesReference species=\"S1\" stoichiometry=\"1\" constant=\"true\"/>\n"
           "        </listOfProducts>\n"
           "        <kineticLaw>\n"
           "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
           "            <apply>\n"
           "              <times/>\n"
           "              <ci> kb </ci>\n"
           "              <ci> S2 </ci>\n"
           "            </apply>\n"
           "          </math>\n"
           "        </kineticLaw>\n"
           "      </reaction>\n"
           "    </listOfReactions>\n"
           "  </model>\n"
           "</sbml>";
}

std::string SimpleFlux::modelName() {
    return "SimpleFlux";
}

ls::DoubleMatrix SimpleFlux::timeSeriesResult() {
    return ls::DoubleMatrix({
                                    {0,        10,      1},
                                    {0.909091, 9.14354, 1.85646},
                                    {1.81818,  8.36858, 2.63142},
                                    {2.72727,  7.66736, 3.33264},
                                    {3.63636,  7.03288, 3.96712},
                                    {4.54545,  6.45878, 4.54122},
                                    {5.45455,  5.9393,  5.0607},
                                    {6.36364,  5.46927, 5.53073},
                                    {7.27273,  5.04396, 5.95604},
                                    {8.18182,  4.65913, 6.34087},
                                    {9.09091,  4.31092, 6.68908},
                                    {10,       3.99584, 7.00416},
                            });
}

std::unordered_map<std::string, rr::Setting> SimpleFlux::timeSeriesSettings() {
    return std::unordered_map<std::string, rr::Setting>{
            {"start",    0},
            {"duration", 10},
            {"steps",    11}
    };
}

StringDoubleMap SimpleFlux::steadyState() {
    return StringDoubleMap(
            {
                    {"S1", 1.0},
                    {"S2", 10.0}
            }
    );
}

std::unordered_map<std::string, rr::Setting> SimpleFlux::steadyStateSettings() {
    return std::unordered_map<std::string, rr::Setting>{
            {"moiety_conservation", true}
    };
}


