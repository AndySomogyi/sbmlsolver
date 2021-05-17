//
// Created by Ciaran on 17/05/2021.
//

#include "CeilInRateLaw.h"


std::string CeilInRateLaw::str() {
    return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
           "<sbml xmlns=\"http://www.sbml.org/sbml/level3/version1/core\" level=\"3\" version=\"1\">\n"
           "  <model metaid=\"CeilModel1\" id=\"CeilModel1\">\n"
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
           "                <ceiling/>\n"
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

std::string CeilInRateLaw::modelName() {
    return "CeilInRateLaw";
}

ls::DoubleMatrix CeilInRateLaw::timeSeriesResult() {
    return ls::DoubleMatrix({
                                    {0,   10,                    0},
                                    {0.4, 3.0119408731651975,    6.9880591268348},
                                    {0.8, 0.9071782094955865,    9.09282179050441},
                                    {1.2, 0.2732365534742893,    9.726763446525707},
                                    {1.6, 0.08229718872631311,   9.917702811273687},
                                    {2,   0.024787413004265253,  9.975212586995736},
                                    {2.4, 0.007465818043742018,  9.992534181956255},
                                    {2.8, 0.002248658948996144,  9.997751341051005},
                                    {3.2, 0.0006772823748483675, 9.999322717625152},
                                    {3.6, 0.0002039933214930827, 9.999796006678507},
                                    {4,   6.144154589603753e-5,  9.999938558454103},
                            });

}

std::unordered_map<std::string, rr::Setting> CeilInRateLaw::timeSeriesSettings() {
    return std::unordered_map<std::string, rr::Setting>{
            {"start",    0},
            {"duration", 4},
            {"steps",    10}
    };
}

