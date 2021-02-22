#include <vector>
#include <unordered_map>
/**
 * This is a small module for handling SBML content
 * without introducing external dependencies. Most
 * of the models contained in this Factory were already
 * present under the models folder.
 */


/**
 * Abstract type to store sbml string
 */
class SBMLTestModel {
public:

    /**
     * Returns the sbml string for this model
     */
    virtual std::string str() = 0;

    /**
     * Returns the model name. This is used by SBMLTestModelFactory
     * for instantiating instances of SBMLTestModel
     */
    virtual std::string modelName() = 0;

    /**
     * @brief returns the state of the system at t=10 time units. 
     * This method is used for storing the true values of 
     * the model simulation at time point 10. These values 
     * must not be computed by roadrunner (otherwise what 
     * are we testing). Instead an independent sbml simulator 
     * should be used to extract the state vector at t=10
     */
    virtual std::unordered_map<std::string, double> stateVectorAtT10() = 0;

};

/**
 * A -> B; k1
 * B -> A; k2
 * k1 = 0.1
 * k2 = 1.0
 * A = 10;
 * B = 0;
 */
class SimpleFlux : public SBMLTestModel {
public:

    std::string str() override {
        return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
               "<sbml xmlns=\"http://www.sbml.org/sbml/level3/version1/core\" level=\"3\" version=\"1\">\n"
               "  <model metaid=\"__main\" id=\"__main\">\n"
               "    <listOfCompartments>\n"
               "      <compartment sboTerm=\"SBO:0000410\" id=\"default_compartment\" spatialDimensions=\"3\" size=\"1\" constant=\"true\"/>\n"
               "    </listOfCompartments>\n"
               "    <listOfSpecies>\n"
               "      <species id=\"A\" compartment=\"default_compartment\" initialConcentration=\"10\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
               "      <species id=\"B\" compartment=\"default_compartment\" initialConcentration=\"0\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
               "    </listOfSpecies>\n"
               "    <listOfParameters>\n"
               "      <parameter id=\"k1\" value=\"0.1\" constant=\"true\"/>\n"
               "      <parameter id=\"k2\" value=\"1\" constant=\"true\"/>\n"
               "    </listOfParameters>\n"
               "    <listOfReactions>\n"
               "      <reaction id=\"_J0\" reversible=\"true\" fast=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"A\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfReactants>\n"
               "        <listOfProducts>\n"
               "          <speciesReference species=\"B\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfProducts>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <times/>\n"
               "              <ci> k1 </ci>\n"
               "              <ci> A </ci>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "      <reaction id=\"_J1\" reversible=\"true\" fast=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"B\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfReactants>\n"
               "        <listOfProducts>\n"
               "          <speciesReference species=\"A\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfProducts>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <times/>\n"
               "              <ci> k2 </ci>\n"
               "              <ci> B </ci>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "    </listOfReactions>\n"
               "  </model>\n"
               "</sbml>";
    }

    std::string modelName() override {
        return "SimpleFlux";
    }

    std::unordered_map<std::string, double> stateVectorAtT10() override {
        return std::unordered_map<std::string, double>
                {
                        {"A", 9.090924288825768},
                        {"B", 0.9090757111742356}
                };

    }
};

/**
 * This is the feedback model from test/models/feedback.xml
 */
class Feedback : public SBMLTestModel {
public:

    std::string str() override {
        return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
               "<sbml xmlns=\"http://www.sbml.org/sbml/level2\" xmlns:jd2=\"http://www.sys-bio.org/sbml/jd2\" level=\"2\" version=\"1\">\n"
               "  <model id=\"feedback\" name=\"Feedback\">\n"
               "    <annotation>\n"
               "      <jd2:JDesignerLayout xmlns:jd2=\"http://www.sys-bio.org/sbml/jd2\" version=\"2.0\" MajorVersion=\"2\" MinorVersion=\"3\" BuildVersion=\"45\">\n"
               "        <jd2:header>\n"
               "          <jd2:VersionHeader JDesignerVersion=\"1.0\"/>\n"
               "          <jd2:ModelHeader ModelTitle=\"Feedback\"/>\n"
               "          <jd2:TimeCourseDetails timeStart=\"0\" timeEnd=\"25\" numberOfPoints=\"200\"/>\n"
               "        </jd2:header>\n"
               "        <jd2:JDGraphicsHeader BackGroundColor=\"FFFFFFEF\"/>\n"
               "        <jd2:lisatOfCompartments>\n"
               "          <jd2:compartment id=\"compartment\" size=\"1\" visible=\"false\">\n"
               "            <jd2:boundingBox x=\"0\" y=\"0\" w=\"0\" h=\"0\"/>\n"
               "            <jd2:membraneStyle thickness=\"12\" color=\"FF00A5FF\"/>\n"
               "            <jd2:interiorStyle color=\"FFEEEEFF\"/>\n"
               "            <jd2:text value=\"compartment\" visible=\"true\">\n"
               "              <jd2:position rx=\"-31\" ry=\"-16\"/>\n"
               "              <jd2:font fontName=\"Arial\" fontSize=\"8\" fontColor=\"FF000000\"/>\n"
               "            </jd2:text>\n"
               "          </jd2:compartment>\n"
               "        </jd2:lisatOfCompartments>\n"
               "        <jd2:listOfSpecies>\n"
               "          <jd2:species id=\"S1\" boundaryCondition=\"false\" compartment=\"compartment\" initialConcentration=\"0\">\n"
               "            <jd2:positionLocked value=\"false\"/>\n"
               "            <jd2:boundingBox x=\"138\" y=\"104\"/>\n"
               "            <jd2:displayValue visible=\"false\">\n"
               "              <jd2:position rx=\"0\" ry=\"0\"/>\n"
               "              <jd2:font fontName=\"Arial\" fontSize=\"8\" fontColor=\"FF000000\"/>\n"
               "            </jd2:displayValue>\n"
               "            <jd2:text value=\"S1\" visible=\"true\">\n"
               "              <jd2:position rx=\"4\" ry=\"6\"/>\n"
               "              <jd2:font fontName=\"Arial\" fontSize=\"8\" fontColor=\"FF000000\"/>\n"
               "            </jd2:text>\n"
               "            <jd2:complex id=\"DefaultMolecule\" w=\"24\" h=\"24\" boundarySpeciesStyle=\"bsBox\" boundaryStyleColor=\"FF0000FF\" captionPosition=\"npCenter\" aliasBoundaryColor=\"FFFF0000\" aliasBoundaryThickness=\"3\">\n"
               "              <jd2:subunit shape=\"suRoundSquare\">\n"
               "                <jd2:boundingBox rx=\"0\" ry=\"0\" w=\"24\" h=\"24\"/>\n"
               "                <jd2:text value=\"S1\" visible=\"false\">\n"
               "                  <jd2:position rx=\"6\" ry=\"5\"/>\n"
               "                  <jd2:font fontName=\"Arial\" fontSize=\"8\" fontColor=\"FF000000\"/>\n"
               "                </jd2:text>\n"
               "                <jd2:color scheme=\"gtHorizLinear\" startColor=\"FFCCFFFF\" endColor=\"FFFFFFFF\"/>\n"
               "                <jd2:edgeStyle color=\"FF969696\" thickness=\"1\" stroke=\"dsSolid\"/>\n"
               "              </jd2:subunit>\n"
               "            </jd2:complex>\n"
               "          </jd2:species>\n"
               "          <jd2:species id=\"S2\" boundaryCondition=\"false\" compartment=\"compartment\" initialConcentration=\"0\">\n"
               "            <jd2:positionLocked value=\"false\"/>\n"
               "            <jd2:boundingBox x=\"234\" y=\"143\"/>\n"
               "            <jd2:displayValue visible=\"false\">\n"
               "              <jd2:position rx=\"0\" ry=\"0\"/>\n"
               "              <jd2:font fontName=\"Arial\" fontSize=\"8\" fontColor=\"FF000000\"/>\n"
               "            </jd2:displayValue>\n"
               "            <jd2:text value=\"S2\" visible=\"true\">\n"
               "              <jd2:position rx=\"4\" ry=\"6\"/>\n"
               "              <jd2:font fontName=\"Arial\" fontSize=\"8\" fontColor=\"FF000000\"/>\n"
               "            </jd2:text>\n"
               "            <jd2:complex id=\"DefaultMolecule\" w=\"24\" h=\"24\" boundarySpeciesStyle=\"bsBox\" boundaryStyleColor=\"FF0000FF\" captionPosition=\"npCenter\" aliasBoundaryColor=\"FFFF0000\" aliasBoundaryThickness=\"3\">\n"
               "              <jd2:subunit shape=\"suRoundSquare\">\n"
               "                <jd2:boundingBox rx=\"0\" ry=\"0\" w=\"24\" h=\"24\"/>\n"
               "                <jd2:text value=\"S1\" visible=\"false\">\n"
               "                  <jd2:position rx=\"6\" ry=\"5\"/>\n"
               "                  <jd2:font fontName=\"Arial\" fontSize=\"8\" fontColor=\"FF000000\"/>\n"
               "                </jd2:text>\n"
               "                <jd2:color scheme=\"gtHorizLinear\" startColor=\"FFCCFFFF\" endColor=\"FFFFFFFF\"/>\n"
               "                <jd2:edgeStyle color=\"FF969696\" thickness=\"1\" stroke=\"dsSolid\"/>\n"
               "              </jd2:subunit>\n"
               "            </jd2:complex>\n"
               "          </jd2:species>\n"
               "          <jd2:species id=\"S3\" boundaryCondition=\"false\" compartment=\"compartment\" initialConcentration=\"0\">\n"
               "            <jd2:positionLocked value=\"false\"/>\n"
               "            <jd2:boundingBox x=\"318\" y=\"178\"/>\n"
               "            <jd2:displayValue visible=\"false\">\n"
               "              <jd2:position rx=\"0\" ry=\"0\"/>\n"
               "              <jd2:font fontName=\"Arial\" fontSize=\"8\" fontColor=\"FF000000\"/>\n"
               "            </jd2:displayValue>\n"
               "            <jd2:text value=\"S3\" visible=\"true\">\n"
               "              <jd2:position rx=\"4\" ry=\"6\"/>\n"
               "              <jd2:font fontName=\"Arial\" fontSize=\"8\" fontColor=\"FF000000\"/>\n"
               "            </jd2:text>\n"
               "            <jd2:complex id=\"DefaultMolecule\" w=\"24\" h=\"24\" boundarySpeciesStyle=\"bsBox\" boundaryStyleColor=\"FF0000FF\" captionPosition=\"npCenter\" aliasBoundaryColor=\"FFFF0000\" aliasBoundaryThickness=\"3\">\n"
               "              <jd2:subunit shape=\"suRoundSquare\">\n"
               "                <jd2:boundingBox rx=\"0\" ry=\"0\" w=\"24\" h=\"24\"/>\n"
               "                <jd2:text value=\"S1\" visible=\"false\">\n"
               "                  <jd2:position rx=\"6\" ry=\"5\"/>\n"
               "                  <jd2:font fontName=\"Arial\" fontSize=\"8\" fontColor=\"FF000000\"/>\n"
               "                </jd2:text>\n"
               "                <jd2:color scheme=\"gtHorizLinear\" startColor=\"FFCCFFFF\" endColor=\"FFFFFFFF\"/>\n"
               "                <jd2:edgeStyle color=\"FF969696\" thickness=\"1\" stroke=\"dsSolid\"/>\n"
               "              </jd2:subunit>\n"
               "            </jd2:complex>\n"
               "          </jd2:species>\n"
               "          <jd2:species id=\"S4\" boundaryCondition=\"false\" compartment=\"compartment\" initialConcentration=\"0\">\n"
               "            <jd2:positionLocked value=\"false\"/>\n"
               "            <jd2:boundingBox x=\"411\" y=\"212\"/>\n"
               "            <jd2:displayValue visible=\"false\">\n"
               "              <jd2:position rx=\"0\" ry=\"0\"/>\n"
               "              <jd2:font fontName=\"Arial\" fontSize=\"8\" fontColor=\"FF000000\"/>\n"
               "            </jd2:displayValue>\n"
               "            <jd2:text value=\"S4\" visible=\"true\">\n"
               "              <jd2:position rx=\"4\" ry=\"6\"/>\n"
               "              <jd2:font fontName=\"Arial\" fontSize=\"8\" fontColor=\"FF000000\"/>\n"
               "            </jd2:text>\n"
               "            <jd2:complex id=\"DefaultMolecule\" w=\"24\" h=\"24\" boundarySpeciesStyle=\"bsBox\" boundaryStyleColor=\"FF0000FF\" captionPosition=\"npCenter\" aliasBoundaryColor=\"FFFF0000\" aliasBoundaryThickness=\"3\">\n"
               "              <jd2:subunit shape=\"suRoundSquare\">\n"
               "                <jd2:boundingBox rx=\"0\" ry=\"0\" w=\"24\" h=\"24\"/>\n"
               "                <jd2:text value=\"S1\" visible=\"false\">\n"
               "                  <jd2:position rx=\"6\" ry=\"5\"/>\n"
               "                  <jd2:font fontName=\"Arial\" fontSize=\"8\" fontColor=\"FF000000\"/>\n"
               "                </jd2:text>\n"
               "                <jd2:color scheme=\"gtHorizLinear\" startColor=\"FFCCFFFF\" endColor=\"FFFFFFFF\"/>\n"
               "                <jd2:edgeStyle color=\"FF969696\" thickness=\"1\" stroke=\"dsSolid\"/>\n"
               "              </jd2:subunit>\n"
               "            </jd2:complex>\n"
               "          </jd2:species>\n"
               "          <jd2:species id=\"X0\" boundaryCondition=\"true\" compartment=\"compartment\" initialConcentration=\"10\">\n"
               "            <jd2:positionLocked value=\"false\"/>\n"
               "            <jd2:boundingBox x=\"33\" y=\"60\"/>\n"
               "            <jd2:displayValue visible=\"false\">\n"
               "              <jd2:position rx=\"0\" ry=\"0\"/>\n"
               "              <jd2:font fontName=\"Arial\" fontSize=\"8\" fontColor=\"FF000000\"/>\n"
               "            </jd2:displayValue>\n"
               "            <jd2:text value=\"X0\" visible=\"true\">\n"
               "              <jd2:position rx=\"4\" ry=\"6\"/>\n"
               "              <jd2:font fontName=\"Arial\" fontSize=\"8\" fontColor=\"FF000000\"/>\n"
               "            </jd2:text>\n"
               "            <jd2:complex id=\"DefaultMolecule\" w=\"24\" h=\"24\" boundarySpeciesStyle=\"bsBox\" boundaryStyleColor=\"FF0000FF\" captionPosition=\"npCenter\" aliasBoundaryColor=\"FFFF0000\" aliasBoundaryThickness=\"3\">\n"
               "              <jd2:subunit shape=\"suRoundSquare\">\n"
               "                <jd2:boundingBox rx=\"0\" ry=\"0\" w=\"24\" h=\"24\"/>\n"
               "                <jd2:text value=\"S1\" visible=\"false\">\n"
               "                  <jd2:position rx=\"6\" ry=\"5\"/>\n"
               "                  <jd2:font fontName=\"Arial\" fontSize=\"8\" fontColor=\"FF000000\"/>\n"
               "                </jd2:text>\n"
               "                <jd2:color scheme=\"gtHorizLinear\" startColor=\"FFCCFFFF\" endColor=\"FFFFFFFF\"/>\n"
               "                <jd2:edgeStyle color=\"FF969696\" thickness=\"1\" stroke=\"dsSolid\"/>\n"
               "              </jd2:subunit>\n"
               "            </jd2:complex>\n"
               "          </jd2:species>\n"
               "          <jd2:species id=\"X1\" boundaryCondition=\"true\" compartment=\"compartment\" initialConcentration=\"0\">\n"
               "            <jd2:positionLocked value=\"false\"/>\n"
               "            <jd2:boundingBox x=\"415\" y=\"292\"/>\n"
               "            <jd2:displayValue visible=\"false\">\n"
               "              <jd2:position rx=\"0\" ry=\"0\"/>\n"
               "              <jd2:font fontName=\"Arial\" fontSize=\"8\" fontColor=\"FF000000\"/>\n"
               "            </jd2:displayValue>\n"
               "            <jd2:text value=\"X1\" visible=\"true\">\n"
               "              <jd2:position rx=\"4\" ry=\"6\"/>\n"
               "              <jd2:font fontName=\"Arial\" fontSize=\"8\" fontColor=\"FF000000\"/>\n"
               "            </jd2:text>\n"
               "            <jd2:complex id=\"DefaultMolecule\" w=\"24\" h=\"24\" boundarySpeciesStyle=\"bsBox\" boundaryStyleColor=\"FF0000FF\" captionPosition=\"npCenter\" aliasBoundaryColor=\"FFFF0000\" aliasBoundaryThickness=\"3\">\n"
               "              <jd2:subunit shape=\"suRoundSquare\">\n"
               "                <jd2:boundingBox rx=\"0\" ry=\"0\" w=\"24\" h=\"24\"/>\n"
               "                <jd2:text value=\"S1\" visible=\"false\">\n"
               "                  <jd2:position rx=\"6\" ry=\"5\"/>\n"
               "                  <jd2:font fontName=\"Arial\" fontSize=\"8\" fontColor=\"FF000000\"/>\n"
               "                </jd2:text>\n"
               "                <jd2:color scheme=\"gtHorizLinear\" startColor=\"FFCCFFFF\" endColor=\"FFFFFFFF\"/>\n"
               "                <jd2:edgeStyle color=\"FF969696\" thickness=\"1\" stroke=\"dsSolid\"/>\n"
               "              </jd2:subunit>\n"
               "            </jd2:complex>\n"
               "          </jd2:species>\n"
               "        </jd2:listOfSpecies>\n"
               "        <jd2:listOfReactions>\n"
               "          <jd2:reaction id=\"J0\" reversible=\"false\">\n"
               "            <jd2:listOfReactants>\n"
               "              <jd2:speciesReference species=\"X0\" stoichiometry=\"1\"/>\n"
               "            </jd2:listOfReactants>\n"
               "            <jd2:listOfProducts>\n"
               "              <jd2:speciesReference species=\"S1\" stoichiometry=\"1\"/>\n"
               "            </jd2:listOfProducts>\n"
               "            <jd2:listOfModifierEdges>\n"
               "              <jd2:modifierEdge>\n"
               "                <jd2:speciesReference species=\"S4\"/>\n"
               "                <jd2:destinationReaction name=\"J0\" regulatorType=\"rtNegative\" relativePosition=\"0.415929203539823\" destinationArcId=\"0\" destinationLineSegmentId=\"0\"/>\n"
               "                <jd2:display lineThickness=\"2\" lineColor=\"FF0000FF\" lineDashStyle=\"dsSolid\" negativeMarkerStyle=\"rmBar\">\n"
               "                  <jd2:lineType type=\"ltSegmentedLine\">\n"
               "                    <jd2:pt x=\"423\" y=\"224\" type=\"modifier\" speciesRef=\"S4\"/>\n"
               "                    <jd2:pt x=\"353\" y=\"243\"/>\n"
               "                    <jd2:pt x=\"69\" y=\"127\"/>\n"
               "                    <jd2:pt x=\"84\" y=\"101\"/>\n"
               "                  </jd2:lineType>\n"
               "                </jd2:display>\n"
               "              </jd2:modifierEdge>\n"
               "            </jd2:listOfModifierEdges>\n"
               "            <jd2:kineticLaw type=\"explicit\">\n"
               "              <jd2:rateEquation value=\"J0_VM1*(X0-S1/J0_Keq1)/(1+X0+S1+pow(S4,J0_h))\"/>\n"
               "            </jd2:kineticLaw>\n"
               "            <jd2:display arrowStyle=\"ar1\" arrowFillColor=\"FF000000\" lineColor=\"FF838464\" lineThickness=\"2\">\n"
               "              <jd2:lineType type=\"ltLine\">\n"
               "                <jd2:edge>\n"
               "                  <jd2:pt x=\"45\" y=\"72\" type=\"substrate\" speciesRef=\"X0\"/>\n"
               "                  <jd2:pt x=\"150\" y=\"116\" type=\"product\" speciesRef=\"S1\"/>\n"
               "                </jd2:edge>\n"
               "              </jd2:lineType>\n"
               "            </jd2:display>\n"
               "          </jd2:reaction>\n"
               "          <jd2:reaction id=\"J1\" reversible=\"false\">\n"
               "            <jd2:listOfReactants>\n"
               "              <jd2:speciesReference species=\"S1\" stoichiometry=\"1\"/>\n"
               "            </jd2:listOfReactants>\n"
               "            <jd2:listOfProducts>\n"
               "              <jd2:speciesReference species=\"S2\" stoichiometry=\"1\"/>\n"
               "            </jd2:listOfProducts>\n"
               "            <jd2:listOfModifierEdges/>\n"
               "            <jd2:kineticLaw type=\"explicit\">\n"
               "              <jd2:rateEquation value=\"(10*S1-2*S2)/(1+S1+S2)\"/>\n"
               "            </jd2:kineticLaw>\n"
               "            <jd2:display arrowStyle=\"ar1\" arrowFillColor=\"FF000000\" lineColor=\"FF838464\" lineThickness=\"2\">\n"
               "              <jd2:lineType type=\"ltLine\">\n"
               "                <jd2:edge>\n"
               "                  <jd2:pt x=\"150\" y=\"116\" type=\"substrate\" speciesRef=\"S1\"/>\n"
               "                  <jd2:pt x=\"246\" y=\"155\" type=\"product\" speciesRef=\"S2\"/>\n"
               "                </jd2:edge>\n"
               "              </jd2:lineType>\n"
               "            </jd2:display>\n"
               "          </jd2:reaction>\n"
               "          <jd2:reaction id=\"J2\" reversible=\"false\">\n"
               "            <jd2:listOfReactants>\n"
               "              <jd2:speciesReference species=\"S2\" stoichiometry=\"1\"/>\n"
               "            </jd2:listOfReactants>\n"
               "            <jd2:listOfProducts>\n"
               "              <jd2:speciesReference species=\"S3\" stoichiometry=\"1\"/>\n"
               "            </jd2:listOfProducts>\n"
               "            <jd2:listOfModifierEdges/>\n"
               "            <jd2:kineticLaw type=\"explicit\">\n"
               "              <jd2:rateEquation value=\"(10*S2-2*S3)/(1+S2+S3)\"/>\n"
               "            </jd2:kineticLaw>\n"
               "            <jd2:display arrowStyle=\"ar1\" arrowFillColor=\"FF000000\" lineColor=\"FF838464\" lineThickness=\"2\">\n"
               "              <jd2:lineType type=\"ltLine\">\n"
               "                <jd2:edge>\n"
               "                  <jd2:pt x=\"246\" y=\"155\" type=\"substrate\" speciesRef=\"S2\"/>\n"
               "                  <jd2:pt x=\"330\" y=\"190\" type=\"product\" speciesRef=\"S3\"/>\n"
               "                </jd2:edge>\n"
               "              </jd2:lineType>\n"
               "            </jd2:display>\n"
               "          </jd2:reaction>\n"
               "          <jd2:reaction id=\"J3\" reversible=\"false\">\n"
               "            <jd2:listOfReactants>\n"
               "              <jd2:speciesReference species=\"S3\" stoichiometry=\"1\"/>\n"
               "            </jd2:listOfReactants>\n"
               "            <jd2:listOfProducts>\n"
               "              <jd2:speciesReference species=\"S4\" stoichiometry=\"1\"/>\n"
               "            </jd2:listOfProducts>\n"
               "            <jd2:listOfModifierEdges/>\n"
               "            <jd2:kineticLaw type=\"explicit\">\n"
               "              <jd2:rateEquation value=\"(10*S3-2*S4)/(1+S3+S4)\"/>\n"
               "            </jd2:kineticLaw>\n"
               "            <jd2:display arrowStyle=\"ar1\" arrowFillColor=\"FF000000\" lineColor=\"FF838464\" lineThickness=\"2\">\n"
               "              <jd2:lineType type=\"ltLine\">\n"
               "                <jd2:edge>\n"
               "                  <jd2:pt x=\"330\" y=\"190\" type=\"substrate\" speciesRef=\"S3\"/>\n"
               "                  <jd2:pt x=\"423\" y=\"224\" type=\"product\" speciesRef=\"S4\"/>\n"
               "                </jd2:edge>\n"
               "              </jd2:lineType>\n"
               "            </jd2:display>\n"
               "          </jd2:reaction>\n"
               "          <jd2:reaction id=\"J4\" reversible=\"false\">\n"
               "            <jd2:listOfReactants>\n"
               "              <jd2:speciesReference species=\"S4\" stoichiometry=\"1\"/>\n"
               "            </jd2:listOfReactants>\n"
               "            <jd2:listOfProducts>\n"
               "              <jd2:speciesReference species=\"X1\" stoichiometry=\"1\"/>\n"
               "            </jd2:listOfProducts>\n"
               "            <jd2:listOfModifierEdges/>\n"
               "            <jd2:kineticLaw type=\"explicit\">\n"
               "              <jd2:rateEquation value=\"J4_V4*S4/(J4_KS4+S4)\"/>\n"
               "            </jd2:kineticLaw>\n"
               "            <jd2:display arrowStyle=\"ar1\" arrowFillColor=\"FF000000\" lineColor=\"FF838464\" lineThickness=\"2\">\n"
               "              <jd2:lineType type=\"ltLine\">\n"
               "                <jd2:edge>\n"
               "                  <jd2:pt x=\"423\" y=\"224\" type=\"substrate\" speciesRef=\"S4\"/>\n"
               "                  <jd2:pt x=\"427\" y=\"304\" type=\"product\" speciesRef=\"X1\"/>\n"
               "                </jd2:edge>\n"
               "              </jd2:lineType>\n"
               "            </jd2:display>\n"
               "          </jd2:reaction>\n"
               "        </jd2:listOfReactions>\n"
               "      </jd2:JDesignerLayout>\n"
               "    </annotation>\n"
               "    <listOfCompartments>\n"
               "      <compartment id=\"compartment\" size=\"1\"/>\n"
               "    </listOfCompartments>\n"
               "    <listOfSpecies>\n"
               "      <species id=\"S1\" compartment=\"compartment\" initialConcentration=\"0\"/>\n"
               "      <species id=\"S2\" compartment=\"compartment\" initialConcentration=\"0\"/>\n"
               "      <species id=\"S3\" compartment=\"compartment\" initialConcentration=\"0\"/>\n"
               "      <species id=\"S4\" compartment=\"compartment\" initialConcentration=\"0\"/>\n"
               "      <species id=\"X0\" compartment=\"compartment\" initialConcentration=\"10\" boundaryCondition=\"true\"/>\n"
               "      <species id=\"X1\" compartment=\"compartment\" initialConcentration=\"0\" boundaryCondition=\"true\"/>\n"
               "    </listOfSpecies>\n"
               "    <listOfParameters>\n"
               "      <parameter id=\"J0_VM1\" value=\"10\"/>\n"
               "      <parameter id=\"J0_Keq1\" value=\"10\"/>\n"
               "      <parameter id=\"J0_h\" value=\"10\"/>\n"
               "      <parameter id=\"J4_V4\" value=\"2.5\"/>\n"
               "      <parameter id=\"J4_KS4\" value=\"0.5\"/>\n"
               "    </listOfParameters>\n"
               "    <listOfReactions>\n"
               "      <reaction id=\"J0\" reversible=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"X0\"/>\n"
               "        </listOfReactants>\n"
               "        <listOfProducts>\n"
               "          <speciesReference species=\"S1\"/>\n"
               "        </listOfProducts>\n"
               "        <listOfModifiers>\n"
               "          <modifierSpeciesReference species=\"S4\"/>\n"
               "          <modifierSpeciesReference species=\"S4\"/>\n"
               "        </listOfModifiers>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <divide/>\n"
               "              <apply>\n"
               "                <times/>\n"
               "                <ci> J0_VM1 </ci>\n"
               "                <apply>\n"
               "                  <minus/>\n"
               "                  <ci> X0 </ci>\n"
               "                  <apply>\n"
               "                    <divide/>\n"
               "                    <ci> S1 </ci>\n"
               "                    <ci> J0_Keq1 </ci>\n"
               "                  </apply>\n"
               "                </apply>\n"
               "              </apply>\n"
               "              <apply>\n"
               "                <plus/>\n"
               "                <cn type=\"integer\"> 1 </cn>\n"
               "                <ci> X0 </ci>\n"
               "                <ci> S1 </ci>\n"
               "                <apply>\n"
               "                  <power/>\n"
               "                  <ci> S4 </ci>\n"
               "                  <ci> J0_h </ci>\n"
               "                </apply>\n"
               "              </apply>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "      <reaction id=\"J1\" reversible=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"S1\"/>\n"
               "        </listOfReactants>\n"
               "        <listOfProducts>\n"
               "          <speciesReference species=\"S2\"/>\n"
               "        </listOfProducts>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <divide/>\n"
               "              <apply>\n"
               "                <minus/>\n"
               "                <apply>\n"
               "                  <times/>\n"
               "                  <cn type=\"integer\"> 10 </cn>\n"
               "                  <ci> S1 </ci>\n"
               "                </apply>\n"
               "                <apply>\n"
               "                  <times/>\n"
               "                  <cn type=\"integer\"> 2 </cn>\n"
               "                  <ci> S2 </ci>\n"
               "                </apply>\n"
               "              </apply>\n"
               "              <apply>\n"
               "                <plus/>\n"
               "                <cn type=\"integer\"> 1 </cn>\n"
               "                <ci> S1 </ci>\n"
               "                <ci> S2 </ci>\n"
               "              </apply>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "      <reaction id=\"J2\" reversible=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"S2\"/>\n"
               "        </listOfReactants>\n"
               "        <listOfProducts>\n"
               "          <speciesReference species=\"S3\"/>\n"
               "        </listOfProducts>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <divide/>\n"
               "              <apply>\n"
               "                <minus/>\n"
               "                <apply>\n"
               "                  <times/>\n"
               "                  <cn type=\"integer\"> 10 </cn>\n"
               "                  <ci> S2 </ci>\n"
               "                </apply>\n"
               "                <apply>\n"
               "                  <times/>\n"
               "                  <cn type=\"integer\"> 2 </cn>\n"
               "                  <ci> S3 </ci>\n"
               "                </apply>\n"
               "              </apply>\n"
               "              <apply>\n"
               "                <plus/>\n"
               "                <cn type=\"integer\"> 1 </cn>\n"
               "                <ci> S2 </ci>\n"
               "                <ci> S3 </ci>\n"
               "              </apply>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "      <reaction id=\"J3\" reversible=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"S3\"/>\n"
               "        </listOfReactants>\n"
               "        <listOfProducts>\n"
               "          <speciesReference species=\"S4\"/>\n"
               "        </listOfProducts>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <divide/>\n"
               "              <apply>\n"
               "                <minus/>\n"
               "                <apply>\n"
               "                  <times/>\n"
               "                  <cn type=\"integer\"> 10 </cn>\n"
               "                  <ci> S3 </ci>\n"
               "                </apply>\n"
               "                <apply>\n"
               "                  <times/>\n"
               "                  <cn type=\"integer\"> 2 </cn>\n"
               "                  <ci> S4 </ci>\n"
               "                </apply>\n"
               "              </apply>\n"
               "              <apply>\n"
               "                <plus/>\n"
               "                <cn type=\"integer\"> 1 </cn>\n"
               "                <ci> S3 </ci>\n"
               "                <ci> S4 </ci>\n"
               "              </apply>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "      <reaction id=\"J4\" reversible=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"S4\"/>\n"
               "        </listOfReactants>\n"
               "        <listOfProducts>\n"
               "          <speciesReference species=\"X1\"/>\n"
               "        </listOfProducts>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <divide/>\n"
               "              <apply>\n"
               "                <times/>\n"
               "                <ci> J4_V4 </ci>\n"
               "                <ci> S4 </ci>\n"
               "              </apply>\n"
               "              <apply>\n"
               "                <plus/>\n"
               "                <ci> J4_KS4 </ci>\n"
               "                <ci> S4 </ci>\n"
               "              </apply>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "    </listOfReactions>\n"
               "  </model>\n"
               "</sbml>";
    }

    std::string modelName() override {
        return "Feedback";
    }

    std::unordered_map<std::string, double> stateVectorAtT10() override {
        return std::unordered_map<std::string, double>
                {
                        {"S1", 0.26942985546551135},
                        {"S2", 0.678092484380656},
                        {"S3", 1.1993552974554607},
                        {"S4", 1.8682526589726127}
                };

    }
};


/**
 * model 269 from the sbml test suite
 */
class Model269 : public SBMLTestModel {
public:

    std::string str() override {
        return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
               "<sbml xmlns=\"http://www.sbml.org/sbml/level3/version2/core\" level=\"3\" version=\"2\">\n"
               "  <model metaid=\"_case00269\" id=\"case00269\" name=\"case00269\" timeUnits=\"time\">\n"
               "    <listOfFunctionDefinitions>\n"
               "      <functionDefinition id=\"calculate\" name=\"calculate\">\n"
               "        <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "          <lambda>\n"
               "            <bvar>\n"
               "              <ci> x </ci>\n"
               "            </bvar>\n"
               "            <bvar>\n"
               "              <ci> y </ci>\n"
               "            </bvar>\n"
               "            <bvar>\n"
               "              <ci> z </ci>\n"
               "            </bvar>\n"
               "            <apply>\n"
               "              <times/>\n"
               "              <apply>\n"
               "                <factorial/>\n"
               "                <apply>\n"
               "                  <ceiling/>\n"
               "                  <apply>\n"
               "                    <times/>\n"
               "                    <ci> x </ci>\n"
               "                    <ci> y </ci>\n"
               "                  </apply>\n"
               "                </apply>\n"
               "              </apply>\n"
               "              <apply>\n"
               "                <power/>\n"
               "                <ci> z </ci>\n"
               "                <cn type=\"integer\"> -1 </cn>\n"
               "              </apply>\n"
               "            </apply>\n"
               "          </lambda>\n"
               "        </math>\n"
               "      </functionDefinition>\n"
               "    </listOfFunctionDefinitions>\n"
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
               "              <ci> calculate </ci>\n"
               "              <ci> S1 </ci>\n"
               "              <ci> p1 </ci>\n"
               "              <ci> p2 </ci>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "    </listOfReactions>\n"
               "  </model>\n"
               "</sbml>";
    }

    std::string modelName() override {
        return "Model269";
    }

    std::unordered_map<std::string, double> stateVectorAtT10() override {
        return std::unordered_map<std::string, double>{
                {"S1", 0.0270834},
                {"S2", 0.972917}
        };
    }
};


/**
 * A model that uses "ceil" in the rate law
 */
class CeilInRateLaw : public SBMLTestModel {
public:

    std::string str() override {
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

    std::string modelName() override {
        return "CeilInRateLaw";
    }

    std::unordered_map<std::string, double> stateVectorAtT10() override {
        return std::unordered_map<std::string, double>{
                {"S1", 9.02844e-13},
                {"S2", 10}
        };
    }
};

/**
 * A model that uses "Factorial" in the rate law
 */
class FactorialInRateLaw : public SBMLTestModel {
public:

    std::string str() override {
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

    std::string modelName() override {
        return "FactorialInRateLaw";
    }

    std::unordered_map<std::string, double> stateVectorAtT10() override {
        return std::unordered_map<std::string, double>{
                {"S1", 1.46671e-12},
                {"S2", 10}
        };
    }
};

//1517

/**
 * Basic factory that creates sbml strings
 * for use in tests.
 *
 * The caller is responsible for deleting memory
 * associated with the createe SBMLTestModel
 */
SBMLTestModel *SBMLTestModelFactory(const std::string &modelName) {
    if (modelName == "SimpleFlux") {
        return new SimpleFlux();
    } else if (modelName == "Feedback") {
        return new Feedback();
    } else if (modelName == "Model269") {
        return new Model269();
    } else if (modelName == "CeilInRateLaw") {
        return new CeilInRateLaw();
    } else if (modelName == "FactorialInRateLaw") {
        return new FactorialInRateLaw();
    } else {
        throw std::runtime_error("SBMLTestModelFactory::SBMLTestModelFactory(): no model found\n");
    }
}













