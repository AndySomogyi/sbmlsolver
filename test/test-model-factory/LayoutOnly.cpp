//
// Created by Ciaran on 17/05/2021.
//

#include "LayoutOnly.h"



std::string LayoutOnly::str() {
    return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
           "<sbml xmlns=\"http://www.sbml.org/sbml/level3/version1/core\" xmlns:layout=\"http://www.sbml.org/sbml/level3/version1/layout/version1\" level=\"3\" version=\"1\" layout:required=\"false\">\n"
           "  <model id=\"TestModel\">\n"
           "    <listOfCompartments>\n"
           "      <compartment id=\"Compartment_1\" constant=\"true\"/>\n"
           "    </listOfCompartments>\n"
           "    <listOfSpecies>\n"
           "      <species id=\"Species_1\" compartment=\"Compartment_1\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
           "      <species id=\"Species_2\" compartment=\"Compartment_1\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
           "    </listOfSpecies>\n"
           "    <listOfReactions>\n"
           "      <reaction id=\"Reaction_1\" reversible=\"false\" fast=\"false\">\n"
           "        <listOfReactants>\n"
           "          <speciesReference id=\"SpeciesReference_1\" species=\"Species_1\" constant=\"false\"/>\n"
           "        </listOfReactants>\n"
           "        <listOfProducts>\n"
           "          <speciesReference id=\"SpeciesReference_2\" species=\"Species_2\" constant=\"false\"/>\n"
           "        </listOfProducts>\n"
           "      </reaction>\n"
           "      <reaction id=\"Reaction_2\" reversible=\"false\" fast=\"false\">\n"
           "        <listOfReactants>\n"
           "          <speciesReference id=\"SpeciesReference_3\" species=\"Species_2\" constant=\"false\"/>\n"
           "        </listOfReactants>\n"
           "        <listOfProducts>\n"
           "          <speciesReference id=\"SpeciesReference_4\" species=\"Species_1\" constant=\"false\"/>\n"
           "        </listOfProducts>\n"
           "      </reaction>\n"
           "    </listOfReactions>\n"
           "    <layout:listOfLayouts xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:layout=\"http://www.sbml.org/sbml/level3/version1/layout/version1\">\n"
           "      <layout:layout layout:id=\"Layout_1\">\n"
           "        <layout:dimensions layout:width=\"400\" layout:height=\"220\"/>\n"
           "        <layout:listOfCompartmentGlyphs>\n"
           "          <layout:compartmentGlyph layout:id=\"CompartmentGlyph_1\" layout:compartment=\"Compartment_1\">\n"
           "            <layout:boundingBox layout:id=\"bb1\">\n"
           "              <layout:position layout:x=\"5\" layout:y=\"5\"/>\n"
           "              <layout:dimensions layout:width=\"390\" layout:height=\"210\"/>\n"
           "            </layout:boundingBox>\n"
           "          </layout:compartmentGlyph>\n"
           "        </layout:listOfCompartmentGlyphs>\n"
           "        <layout:listOfSpeciesGlyphs>\n"
           "          <layout:speciesGlyph layout:id=\"SpeciesGlyph_1\" layout:species=\"Species_1\">\n"
           "            <layout:boundingBox layout:id=\"bb2\">\n"
           "              <layout:position layout:x=\"80\" layout:y=\"26\"/>\n"
           "              <layout:dimensions layout:width=\"240\" layout:height=\"24\"/>\n"
           "            </layout:boundingBox>\n"
           "          </layout:speciesGlyph>\n"
           "          <layout:speciesGlyph layout:id=\"SpeciesGlyph_2\" layout:species=\"Species_2\">\n"
           "            <layout:boundingBox layout:id=\"bb3\">\n"
           "              <layout:position layout:x=\"80\" layout:y=\"170\"/>\n"
           "              <layout:dimensions layout:width=\"240\" layout:height=\"24\"/>\n"
           "            </layout:boundingBox>\n"
           "          </layout:speciesGlyph>\n"
           "        </layout:listOfSpeciesGlyphs>\n"
           "        <layout:listOfReactionGlyphs>\n"
           "          <layout:reactionGlyph layout:id=\"ReactionGlyph_1\" layout:reaction=\"Reaction_1\">\n"
           "            <layout:curve>\n"
           "              <layout:listOfCurveSegments>\n"
           "                <layout:curveSegment xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:type=\"LineSegment\">\n"
           "                  <layout:start layout:x=\"165\" layout:y=\"105\"/>\n"
           "                  <layout:end layout:x=\"165\" layout:y=\"115\"/>\n"
           "                </layout:curveSegment>\n"
           "              </layout:listOfCurveSegments>\n"
           "            </layout:curve>\n"
           "            <layout:listOfSpeciesReferenceGlyphs>\n"
           "              <layout:speciesReferenceGlyph layout:id=\"SpeciesReferenceGlyph_1\" layout:speciesReference=\"SpeciesReference_1\" layout:speciesGlyph=\"SpeciesGlyph_1\" layout:role=\"substrate\">\n"
           "                <layout:curve>\n"
           "                  <layout:listOfCurveSegments>\n"
           "                    <layout:curveSegment xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:type=\"CubicBezier\">\n"
           "                      <layout:start layout:x=\"165\" layout:y=\"105\"/>\n"
           "                      <layout:end layout:x=\"195\" layout:y=\"60\"/>\n"
           "                      <layout:basePoint1 layout:x=\"165\" layout:y=\"90\"/>\n"
           "                      <layout:basePoint2 layout:x=\"165\" layout:y=\"90\"/>\n"
           "                    </layout:curveSegment>\n"
           "                  </layout:listOfCurveSegments>\n"
           "                </layout:curve>\n"
           "              </layout:speciesReferenceGlyph>\n"
           "              <layout:speciesReferenceGlyph layout:id=\"SpeciesReferenceGlyph_2\" layout:speciesReference=\"SpeciesReference_2\" layout:speciesGlyph=\"SpeciesGlyph_2\" layout:role=\"product\">\n"
           "                <layout:curve>\n"
           "                  <layout:listOfCurveSegments>\n"
           "                    <layout:curveSegment xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:type=\"CubicBezier\">\n"
           "                      <layout:start layout:x=\"165\" layout:y=\"115\"/>\n"
           "                      <layout:end layout:x=\"195\" layout:y=\"160\"/>\n"
           "                      <layout:basePoint1 layout:x=\"165\" layout:y=\"130\"/>\n"
           "                      <layout:basePoint2 layout:x=\"165\" layout:y=\"130\"/>\n"
           "                    </layout:curveSegment>\n"
           "                  </layout:listOfCurveSegments>\n"
           "                </layout:curve>\n"
           "              </layout:speciesReferenceGlyph>\n"
           "            </layout:listOfSpeciesReferenceGlyphs>\n"
           "          </layout:reactionGlyph>\n"
           "          <layout:reactionGlyph layout:id=\"ReactionGlyph_2\" layout:reaction=\"Reaction_2\">\n"
           "            <layout:curve>\n"
           "              <layout:listOfCurveSegments>\n"
           "                <layout:curveSegment xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:type=\"LineSegment\">\n"
           "                  <layout:start layout:x=\"235\" layout:y=\"105\"/>\n"
           "                  <layout:end layout:x=\"235\" layout:y=\"115\"/>\n"
           "                </layout:curveSegment>\n"
           "              </layout:listOfCurveSegments>\n"
           "            </layout:curve>\n"
           "            <layout:listOfSpeciesReferenceGlyphs>\n"
           "              <layout:speciesReferenceGlyph layout:id=\"SpeciesReferenceGlyph_3\" layout:speciesReference=\"SpeciesReference_3\" layout:speciesGlyph=\"SpeciesGlyph_2\" layout:role=\"substrate\">\n"
           "                <layout:curve>\n"
           "                  <layout:listOfCurveSegments>\n"
           "                    <layout:curveSegment xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:type=\"CubicBezier\">\n"
           "                      <layout:start layout:x=\"235\" layout:y=\"115\"/>\n"
           "                      <layout:end layout:x=\"205\" layout:y=\"160\"/>\n"
           "                      <layout:basePoint1 layout:x=\"235\" layout:y=\"130\"/>\n"
           "                      <layout:basePoint2 layout:x=\"235\" layout:y=\"130\"/>\n"
           "                    </layout:curveSegment>\n"
           "                  </layout:listOfCurveSegments>\n"
           "                </layout:curve>\n"
           "              </layout:speciesReferenceGlyph>\n"
           "              <layout:speciesReferenceGlyph layout:id=\"SpeciesReferenceGlyph_4\" layout:speciesReference=\"SpeciesReference_4\" layout:speciesGlyph=\"SpeciesGlyph_1\" layout:role=\"product\">\n"
           "                <layout:curve>\n"
           "                  <layout:listOfCurveSegments>\n"
           "                    <layout:curveSegment xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:type=\"CubicBezier\">\n"
           "                      <layout:start layout:x=\"235\" layout:y=\"105\"/>\n"
           "                      <layout:end layout:x=\"205\" layout:y=\"60\"/>\n"
           "                      <layout:basePoint1 layout:x=\"235\" layout:y=\"90\"/>\n"
           "                      <layout:basePoint2 layout:x=\"235\" layout:y=\"90\"/>\n"
           "                    </layout:curveSegment>\n"
           "                  </layout:listOfCurveSegments>\n"
           "                </layout:curve>\n"
           "              </layout:speciesReferenceGlyph>\n"
           "            </layout:listOfSpeciesReferenceGlyphs>\n"
           "          </layout:reactionGlyph>\n"
           "        </layout:listOfReactionGlyphs>\n"
           "        <layout:listOfTextGlyphs>\n"
           "          <layout:textGlyph layout:id=\"TextGlyph_01\" layout:originOfText=\"SpeciesGlyph_1\" layout:graphicalObject=\"SpeciesGlyph_1\">\n"
           "            <layout:boundingBox layout:id=\"bbA\">\n"
           "              <layout:position layout:x=\"92\" layout:y=\"26\"/>\n"
           "              <layout:dimensions layout:width=\"228\" layout:height=\"24\"/>\n"
           "            </layout:boundingBox>\n"
           "          </layout:textGlyph>\n"
           "          <layout:textGlyph layout:id=\"TextGlyph_02\" layout:originOfText=\"SpeciesGlyph_2\" layout:graphicalObject=\"SpeciesGlyph_2\">\n"
           "            <layout:boundingBox layout:id=\"bbB\">\n"
           "              <layout:position layout:x=\"92\" layout:y=\"170\"/>\n"
           "              <layout:dimensions layout:width=\"228\" layout:height=\"24\"/>\n"
           "            </layout:boundingBox>\n"
           "          </layout:textGlyph>\n"
           "        </layout:listOfTextGlyphs>\n"
           "      </layout:layout>\n"
           "    </layout:listOfLayouts>\n"
           "  </model>\n"
           "</sbml>";
}

std::string LayoutOnly::modelName() {
    return "LayoutOnly";
}

