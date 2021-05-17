//
// Created by Ciaran on 17/05/2021.
//

#include "FactorialInRateLaw.h"


std::string FactorialInRateLaw::str() {
    return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
           "<sbml xmlns=\"http://www.sbml.org/sbml/level3/version1/core\" level=\"3\" version=\"1\">\n"
           "  <model metaid=\"FactorialModel1\" id=\"FactorialModel1\">\n"
           "    <listOfCompartments>\n"
           "      <compartment sboTerm=\"SBO:0000410\" id=\"default_compartment\" spatialDimensions=\"3\" size=\"1\" constant=\"true\"/>\n"
           "    </listOfCompartments>\n"
           "    <listOfSpecies>\n"
           "      <species id=\"S1\" compartment=\"default_compartment\" initialConcentration=\"10\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
           "      <species id=\"S2\" compartment=\"default_compartment\" initialConcentration=\"0\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
           "    </listOfSpecies>\n"
           "    <listOfParameters>\n"
           "      <parameter id=\"k1\" value=\"104\" constant=\"true\"/>\n"
           "      <parameter id=\"k2\" value=\"39\" constant=\"true\"/>\n"
           "    </listOfParameters>\n"
           "    <listOfReactions>\n"
           "      <reaction id=\"R1\" reversible=\"true\" fast=\"false\">\n"
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
           "              <ci> S1 </ci>\n"
           "              <apply>\n"
           "                <factorial/>\n"
           "                <apply>\n"
           "                  <divide/>\n"
           "                  <ci> k1 </ci>\n"
           "                  <ci> k2 </ci>\n"
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

std::string FactorialInRateLaw::modelName() {
    return "FactorialInRateLaw";
}

ls::DoubleMatrix FactorialInRateLaw::timeSeriesResult() {
    return ls::DoubleMatrix({
                                    {0,   10,        0},
                                    {0.2, 5.52892,   4.47108},
                                    {0.4, 3.0569,    6.9431},
                                    {0.6, 1.69013,   8.30987},
                                    {0.8, 0.93446,   9.06554},
                                    {1,   0.516655,  9.48334},
                                    {1.2, 0.285655,  9.71435},
                                    {1.4, 0.157936,  9.84206},
                                    {1.6, 0.0873215, 9.91268},
                                    {1.8, 0.0482793, 9.95172},
                                    {2,   0.0266933, 9.97331},
                            });

}

std::unordered_map<std::string, rr::Setting> FactorialInRateLaw::timeSeriesSettings() {
    return std::unordered_map<std::string, rr::Setting>{
            {"start",    0},
            {"duration", 2},
            {"steps",    10}
    };
}

