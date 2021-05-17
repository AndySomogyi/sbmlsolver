//
// Created by Ciaran on 17/05/2021.
//

#include "SimpleFluxManuallyReduced.h"



std::string SimpleFluxManuallyReduced::str() {
    return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
           "<sbml xmlns=\"http://www.sbml.org/sbml/level3/version1/core\" level=\"3\" version=\"1\">\n"
           "  <model metaid=\"x\" id=\"x\">\n"
           "    <listOfCompartments>\n"
           "      <compartment sboTerm=\"SBO:0000410\" id=\"default_compartment\" spatialDimensions=\"3\" size=\"1\" constant=\"true\"/>\n"
           "    </listOfCompartments>\n"
           "    <listOfSpecies>\n"
           "      <species id=\"S1\" compartment=\"default_compartment\" initialConcentration=\"10\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
           "    </listOfSpecies>\n"
           "    <listOfParameters>\n"
           "      <parameter id=\"S20\" value=\"1\" constant=\"true\"/>\n"
           "      <parameter id=\"Total\" constant=\"false\"/>\n"
           "      <parameter id=\"kf\" value=\"0.1\" constant=\"true\"/>\n"
           "      <parameter id=\"kb\" value=\"0.01\" constant=\"true\"/>\n"
           "      <parameter id=\"S2\" constant=\"false\"/>\n"
           "    </listOfParameters>\n"
           "    <listOfInitialAssignments>\n"
           "      <initialAssignment symbol=\"Total\">\n"
           "        <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
           "          <apply>\n"
           "            <plus/>\n"
           "            <ci> S1 </ci>\n"
           "            <ci> S20 </ci>\n"
           "          </apply>\n"
           "        </math>\n"
           "      </initialAssignment>\n"
           "    </listOfInitialAssignments>\n"
           "    <listOfRules>\n"
           "      <assignmentRule variable=\"S2\">\n"
           "        <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
           "          <apply>\n"
           "            <minus/>\n"
           "            <ci> Total </ci>\n"
           "            <ci> S1 </ci>\n"
           "          </apply>\n"
           "        </math>\n"
           "      </assignmentRule>\n"
           "    </listOfRules>\n"
           "    <listOfReactions>\n"
           "      <reaction id=\"_J0\" reversible=\"false\" fast=\"false\">\n"
           "        <listOfReactants>\n"
           "          <speciesReference species=\"S1\" stoichiometry=\"1\" constant=\"true\"/>\n"
           "        </listOfReactants>\n"
           "        <kineticLaw>\n"
           "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
           "            <apply>\n"
           "              <plus/>\n"
           "              <apply>\n"
           "                <times/>\n"
           "                <apply>\n"
           "                  <minus/>\n"
           "                  <ci> kf </ci>\n"
           "                </apply>\n"
           "                <ci> S1 </ci>\n"
           "              </apply>\n"
           "              <apply>\n"
           "                <times/>\n"
           "                <ci> kb </ci>\n"
           "                <apply>\n"
           "                  <minus/>\n"
           "                  <ci> Total </ci>\n"
           "                  <ci> S1 </ci>\n"
           "                </apply>\n"
           "              </apply>\n"
           "            </apply>\n"
           "          </math>\n"
           "        </kineticLaw>\n"
           "      </reaction>\n"
           "    </listOfReactions>\n"
           "  </model>\n"
           "</sbml>";
}

std::string SimpleFluxManuallyReduced::modelName() {
    return "SimpleFluxManuallyReduced";
}

StringDoubleMap SimpleFluxManuallyReduced::steadyState() {
    return StringDoubleMap({
                                   {"S1", 1.0}
                           });
}

std::unordered_map<std::string, rr::Setting> SimpleFluxManuallyReduced::steadyStateSettings() {
    return std::unordered_map<std::string, rr::Setting>{
            {"moiety_conservation", false}
    };
}


