//
// Created by Ciaran on 17/05/2021.
//

#include "Model28.h"


std::string Model28::str() {
    return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
           "<sbml xmlns=\"http://www.sbml.org/sbml/level3/version2/core\" level=\"3\" version=\"2\">\n"
           "  <model metaid=\"_case00028\" id=\"case00028\" name=\"case00028\" timeUnits=\"time\">\n"
           "    <listOfUnitDefinitions>\n"
           "      <unitDefinition id=\"volume\">\n"
           "        <listOfUnits>\n"
           "          <unit kind=\"litre\" exponent=\"1\" scale=\"0\" multiplier=\"1\"/>\n"
           "        </listOfUnits>\n"
           "      </unitDefinition>\n"
           "      <unitDefinition id=\"substance\">\n"
           "        <listOfUnits>\n"
           "          <unit kind=\"mole\" exponent=\"1\" scale=\"0\" multiplier=\"1\"/>\n"
           "        </listOfUnits>\n"
           "      </unitDefinition>\n"
           "      <unitDefinition id=\"time\">\n"
           "        <listOfUnits>\n"
           "          <unit kind=\"second\" exponent=\"1\" scale=\"0\" multiplier=\"1\"/>\n"
           "        </listOfUnits>\n"
           "      </unitDefinition>\n"
           "    </listOfUnitDefinitions>\n"
           "    <listOfCompartments>\n"
           "      <compartment id=\"compartment\" name=\"compartment\" spatialDimensions=\"3\" size=\"1\" units=\"volume\" constant=\"true\"/>\n"
           "    </listOfCompartments>\n"
           "    <listOfSpecies>\n"
           "      <species id=\"S1\" name=\"S1\" compartment=\"compartment\" initialAmount=\"1\" substanceUnits=\"substance\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
           "      <species id=\"S2\" name=\"S2\" compartment=\"compartment\" initialAmount=\"0\" substanceUnits=\"substance\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
           "    </listOfSpecies>\n"
           "    <listOfParameters>\n"
           "      <parameter id=\"p1\" name=\"p1\" value=\"4\" constant=\"true\"/>\n"
           "      <parameter id=\"p2\" name=\"p2\" value=\"25\" constant=\"true\"/>\n"
           "    </listOfParameters>\n"
           "    <listOfReactions>\n"
           "      <reaction id=\"reaction1\" name=\"reaction1\" reversible=\"false\">\n"
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
           "              <apply>\n"
           "                <factorial/>\n"
           "                <apply>\n"
           "                  <ceiling/>\n"
           "                  <apply>\n"
           "                    <times/>\n"
           "                    <ci> p1 </ci>\n"
           "                    <ci> S1 </ci>\n"
           "                  </apply>\n"
           "                </apply>\n"
           "              </apply>\n"
           "              <apply>\n"
           "                <power/>\n"
           "                <ci> p2 </ci>\n"
           "                <cn type=\"integer\"> -1 </cn>\n"
           "              </apply>\n"
           "            </apply>\n"
           "          </math>\n"
           "        </kineticLaw>\n"
           "      </reaction>\n"
           "    </listOfReactions>\n"
           "  </model>\n"
           "</sbml>";
}

std::string Model28::modelName() {
    return "Model28";
}

ls::DoubleMatrix Model28::timeSeriesResult() {
    return ls::DoubleMatrix({
                                    {0,  1,                   0},
                                    {1,  0.5724999588105951,  0.4275000411894048},
                                    {2,  0.444166735551334,   0.5558332644486657},
                                    {3,  0.3641667355513341,  0.6358332644486658},
                                    {4,  0.2841667355513341,  0.7158332644486659},
                                    {5,  0.22708341964881273, 0.7729165803511872},
                                    {6,  0.1870834196488127,  0.8129165803511871},
                                    {7,  0.1470834196488127,  0.8529165803511871},
                                    {8,  0.10708341964881266, 0.892916580351187},
                                    {9,  0.06708341964881266, 0.9329165803511872},
                                    {10, 0.02708341964881266, 0.9729165803511871},
                            });

}

std::unordered_map<std::string, rr::Setting> Model28::timeSeriesSettings() {
    return std::unordered_map<std::string, rr::Setting>{
            {"start",    0},
            {"duration", 10},
            {"steps",    10}
    };
}
