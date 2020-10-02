
#include "rrRoadRunner.h"
#include "sbml/SBMLTypes.h"
#include <regex>

using namespace rr;

int main() {

//    libsbml::ASTNode *top_level_ratelaw = libsbml::SBML_parseL3Formula("k*A*B");
//    char* s = libsbml::SBML_formulaToL3String(top_level_ratelaw);
//    std::string s2(s);
//    free(s);
//    std::cout << s2 << std::endl;
////    bool result = std::regex_match(s2, std::regex("\\d$"));
    auto start_time = std::chrono::high_resolution_clock::now();
    auto t1 = std::chrono::high_resolution_clock::now();
    std::string sbml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                       "<sbml xmlns=\"http://www.sbml.org/sbml/level3/version2/core\" level=\"3\" version=\"2\">\n"
                       "  <model>\n"
                       "    <listOfCompartments>\n"
                       "      <compartment id=\"C1\" size=\"0.1\" constant=\"false\"/>\n"
                       "    </listOfCompartments>\n"
                       "    <listOfSpecies>\n"
                       "      <species id=\"S0\" compartment=\"C1\" initialAmount=\"1\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
                       "      <species id=\"S1\" compartment=\"C1\" initialAmount=\"1\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
                       "      <species id=\"S2\" compartment=\"C1\" initialAmount=\"1\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
                       "      <species id=\"S3\" compartment=\"C1\" initialAmount=\"1\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
                       "    </listOfSpecies>\n"
                       "    <listOfParameters>\n"
                       "      <parameter id=\"k1\" value=\"0.1\" constant=\"false\"/>\n"
                       "      <parameter id=\"k2\" value=\"0.1\" constant=\"false\"/>\n"
                       "      <parameter id=\"k3\" value=\"0.1\" constant=\"false\"/>\n"
                       "    </listOfParameters>\n"
                       "    <listOfReactions>\n"
                       "      <reaction id=\"R1\" reversible=\"false\">\n"
                       "        <listOfReactants>\n"
                       "          <speciesReference species=\"S0\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfReactants>\n"
                       "        <listOfProducts>\n"
                       "          <speciesReference species=\"S1\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfProducts>\n"
                       "        <kineticLaw>\n"
                       "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
                       "            <apply>\n"
                       "              <times/>\n"
                       "              <ci> k1 </ci>\n"
                       "              <ci> S0 </ci>\n"
                       "            </apply>\n"
                       "          </math>\n"
                       "        </kineticLaw>\n"
                       "      </reaction>\n"
                       "      <reaction id=\"R2\" reversible=\"false\">\n"
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
                       "              <ci> k2 </ci>\n"
                       "              <ci> S1 </ci>\n"
                       "            </apply>\n"
                       "          </math>\n"
                       "        </kineticLaw>\n"
                       "      </reaction>\n"
                       "      <reaction id=\"R3\" reversible=\"false\">\n"
                       "        <listOfReactants>\n"
                       "          <speciesReference species=\"S2\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfReactants>\n"
                       "        <listOfProducts>\n"
                       "          <speciesReference species=\"S3\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfProducts>\n"
                       "        <kineticLaw>\n"
                       "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
                       "            <apply>\n"
                       "              <times/>\n"
                       "              <ci> k3 </ci>\n"
                       "              <ci> S2 </ci>\n"
                       "            </apply>\n"
                       "          </math>\n"
                       "        </kineticLaw>\n"
                       "      </reaction>\n"
                       "    </listOfReactions>\n"
                       "  </model>\n"
                       "</sbml>";

    RoadRunner rr(3, 2);
    rr.addCompartment("C1", 0.1, false);
    rr.addSpecies("S0", "C1", 1.0, false, false, "", false);
    rr.addSpecies("S1", "C1", 1.0, false, false, "", false);
    rr.addSpecies("S2", "C1", 1.0, false, false, "", false);
    rr.addSpecies("S3", "C1", 1.0, false, false, "", false);
    rr.addParameter("k1", 0.1, false);
    rr.addParameter("k2", 0.1, false);
    rr.addParameter("k3", 0.1, false);
    rr.addReaction("R1", {"S0"}, {"S1"}, "k1*S0", false);
    rr.addReaction("R2", {"S1"}, {"S2"}, "k2*S1", false);
    rr.addReaction("R3", {"S2"}, {"S3"}, "k3*S2", false);
    rr.regenerate();


//    RoadRunner rr_sbml(sbml);

    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << duration << "; " << duration/1000.0;

}