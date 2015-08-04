// == PREAMBLE ================================================

// * Licensed under the Apache License, Version 2.0; see README

// == FILEDOC =================================================

/** @file ss-solver-test.c
* @author JKM
* @date 08/04/2015
* @copyright Apache License, Version 2.0
* @brief Tests the RoadRunner steady state solver from C
**/

#include "rrc_api.h"
#include "rrc_logging_api.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static const char* feedback_sbml =
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<sbml xmlns=\"http://www.sbml.org/sbml/level2\" xmlns:jd2=\"http://www.sys-bio.org/sbml/jd2\" level=\"2\" version=\"1\">\r\n  <model id=\"feedback\" name=\"Feedback\">\r\n    <listOfCompartments>\r\n      <compartment id=\"compartment\" size=\"1\"/>\r\n    </listOfCompartments>\r\n    <listOfSpecies>\r\n      <species id=\"S1\" compartment=\"compartment\" initialConcentration=\"0\"/>\r\n      <species id=\"S2\" compartment=\"compartment\" initialConcentration=\"0\"/>\r\n      <species id=\"S3\" compartment=\"compartment\" initialConcentration=\"0\"/>\r\n      <species id=\"S4\" compartment=\"compartment\" initialConcentration=\"0\"/>\r\n      <species id=\"X0\" compartment=\"compartment\" initialConcentration=\"10\" boundaryCondition=\"true\"/>\r\n      <species id=\"X1\" compartment=\"compartment\" initialConcentration=\"0\" boundaryCondition=\"true\"/>\r\n    </listOfSpecies>\r\n    <listOfParameters>\r\n      <parameter id=\"J0_VM1\" value=\"10\"/>\r\n      <parameter id=\"J0_Keq1\" value=\"10\"/>\r\n      <parameter id=\"J0_h\" value=\"10\"/>\r\n      <parameter id=\"J4_V4\" value=\"2.5\"/>\r\n      <parameter id=\"J4_KS4\" value=\"0.5\"/>\r\n    </listOfParameters>\r\n    <listOfReactions>\r\n      <reaction id=\"J0\" reversible=\"false\">\r\n        <listOfReactants>\r\n          <speciesReference species=\"X0\"/>\r\n        </listOfReactants>\r\n        <listOfProducts>\r\n          <speciesReference species=\"S1\"/>\r\n        </listOfProducts>\r\n        <listOfModifiers>\r\n          <modifierSpeciesReference species=\"S4\"/>\r\n        </listOfModifiers>\r\n        <kineticLaw>\r\n          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\r\n            <apply>\r\n              <divide/>\r\n              <apply>\r\n                <times/>\r\n                <ci> J0_VM1 </ci>\r\n                <apply>\r\n                  <minus/>\r\n                  <ci> X0 </ci>\r\n                  <apply>\r\n                    <divide/>\r\n                    <ci> S1 </ci>\r\n                    <ci> J0_Keq1 </ci>\r\n                  </apply>\r\n                </apply>\r\n              </apply>\r\n              <apply>\r\n                <plus/>\r\n                <cn type=\"integer\"> 1 </cn>\r\n                <ci> X0 </ci>\r\n                <ci> S1 </ci>\r\n                <apply>\r\n                  <power/>\r\n                  <ci> S4 </ci>\r\n                  <ci> J0_h </ci>\r\n                </apply>\r\n              </apply>\r\n            </apply>\r\n          </math>\r\n        </kineticLaw>\r\n      </reaction>\r\n      <reaction id=\"J1\" reversible=\"false\">\r\n        <listOfReactants>\r\n          <speciesReference species=\"S1\"/>\r\n        </listOfReactants>\r\n        <listOfProducts>\r\n          <speciesReference species=\"S2\"/>\r\n        </listOfProducts>\r\n        <kineticLaw>\r\n          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\r\n            <apply>\r\n              <divide/>\r\n              <apply>\r\n                <minus/>\r\n                <apply>\r\n                  <times/>\r\n                  <cn type=\"integer\"> 10 </cn>\r\n                  <ci> S1 </ci>\r\n                </apply>\r\n                <apply>\r\n                  <times/>\r\n                  <cn type=\"integer\"> 2 </cn>\r\n                  <ci> S2 </ci>\r\n                </apply>\r\n              </apply>\r\n              <apply>\r\n                <plus/>\r\n                <cn type=\"integer\"> 1 </cn>\r\n                <ci> S1 </ci>\r\n                <ci> S2 </ci>\r\n              </apply>\r\n            </apply>\r\n          </math>\r\n        </kineticLaw>\r\n      </reaction>\r\n      <reaction id=\"J2\" reversible=\"false\">\r\n        <listOfReactants>\r\n          <speciesReference species=\"S2\"/>\r\n        </listOfReactants>\r\n        <listOfProducts>\r\n          <speciesReference species=\"S3\"/>\r\n        </listOfProducts>\r\n        <kineticLaw>\r\n          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\r\n            <apply>\r\n              <divide/>\r\n              <apply>\r\n                <minus/>\r\n                <apply>\r\n                  <times/>\r\n                  <cn type=\"integer\"> 10 </cn>\r\n                  <ci> S2 </ci>\r\n                </apply>\r\n                <apply>\r\n                  <times/>\r\n                  <cn type=\"integer\"> 2 </cn>\r\n                  <ci> S3 </ci>\r\n                </apply>\r\n              </apply>\r\n              <apply>\r\n                <plus/>\r\n                <cn type=\"integer\"> 1 </cn>\r\n                <ci> S2 </ci>\r\n                <ci> S3 </ci>\r\n              </apply>\r\n            </apply>\r\n          </math>\r\n        </kineticLaw>\r\n      </reaction>\r\n      <reaction id=\"J3\" reversible=\"false\">\r\n        <listOfReactants>\r\n          <speciesReference species=\"S3\"/>\r\n        </listOfReactants>\r\n        <listOfProducts>\r\n          <speciesReference species=\"S4\"/>\r\n        </listOfProducts>\r\n        <kineticLaw>\r\n          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\r\n            <apply>\r\n              <divide/>\r\n              <apply>\r\n                <minus/>\r\n                <apply>\r\n                  <times/>\r\n                  <cn type=\"integer\"> 10 </cn>\r\n                  <ci> S3 </ci>\r\n                </apply>\r\n                <apply>\r\n                  <times/>\r\n                  <cn type=\"integer\"> 2 </cn>\r\n                  <ci> S4 </ci>\r\n                </apply>\r\n              </apply>\r\n              <apply>\r\n                <plus/>\r\n                <cn type=\"integer\"> 1 </cn>\r\n                <ci> S3 </ci>\r\n                <ci> S4 </ci>\r\n              </apply>\r\n            </apply>\r\n          </math>\r\n        </kineticLaw>\r\n      </reaction>\r\n      <reaction id=\"J4\" reversible=\"false\">\r\n        <listOfReactants>\r\n          <speciesReference species=\"S4\"/>\r\n        </listOfReactants>\r\n        <listOfProducts>\r\n          <speciesReference species=\"X1\"/>\r\n        </listOfProducts>\r\n        <kineticLaw>\r\n          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\r\n            <apply>\r\n              <divide/>\r\n              <apply>\r\n                <times/>\r\n                <ci> J4_V4 </ci>\r\n                <ci> S4 </ci>\r\n              </apply>\r\n              <apply>\r\n                <plus/>\r\n                <ci> J4_KS4 </ci>\r\n                <ci> S4 </ci>\r\n              </apply>\r\n            </apply>\r\n          </math>\r\n        </kineticLaw>\r\n      </reaction>\r\n    </listOfReactions>\r\n  </model>\r\n</sbml>\r\n;";

static const char* bistable_sbml =
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<!-- Created by JarnacLite version 1.0.4965.360 on 2013-10-09 15:31 with libSBML version 5.8.0. -->\r\n<sbml xmlns=\"http://www.sbml.org/sbml/level2/version4\" level=\"2\" version=\"4\">\r\n  <model id=\"cell\" name=\"cell\">\r\n    <listOfCompartments>\r\n      <compartment id=\"compartment\" size=\"1\" />\r\n    </listOfCompartments>\r\n    <listOfSpecies>\r\n      <species id=\"Xo\" compartment=\"compartment\" initialConcentration=\"0\" boundaryCondition=\"true\" />\r\n      <species id=\"w\" compartment=\"compartment\" initialConcentration=\"0\" boundaryCondition=\"true\" />\r\n      <species id=\"x\" compartment=\"compartment\" initialConcentration=\"0.05\" boundaryCondition=\"false\" />\r\n    </listOfSpecies>\r\n    <listOfParameters>\r\n      <parameter id=\"k1\" value=\"0.9\" constant=\"true\" />\r\n      <parameter id=\"k2\" value=\"0.3\" constant=\"true\" />\r\n      <parameter id=\"k3\" value=\"0.7\" constant=\"true\" />\r\n    </listOfParameters>\r\n    <listOfReactions>\r\n      <reaction id=\"J1\" reversible=\"false\">\r\n        <listOfReactants>\r\n          <speciesReference species=\"Xo\" stoichiometry=\"1\" />\r\n        </listOfReactants>\r\n        <listOfProducts>\r\n          <speciesReference species=\"x\" stoichiometry=\"1\" />\r\n        </listOfProducts>\r\n        <kineticLaw>\r\n          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\r\n            <apply>\r\n              <plus />\r\n              <cn> 0.1 </cn>\r\n              <apply>\r\n                <divide />\r\n                <apply>\r\n                  <times />\r\n                  <ci> k1 </ci>\r\n                  <apply>\r\n                    <power />\r\n                    <ci> x </ci>\r\n                    <cn type=\"integer\"> 4 </cn>\r\n                  </apply>\r\n                </apply>\r\n                <apply>\r\n                  <plus />\r\n                  <ci> k2 </ci>\r\n                  <apply>\r\n                    <power />\r\n                    <ci> x </ci>\r\n                    <cn type=\"integer\"> 4 </cn>\r\n                  </apply>\r\n                </apply>\r\n              </apply>\r\n            </apply>\r\n          </math>\r\n        </kineticLaw>\r\n      </reaction>\r\n      <reaction id=\"_J1\" reversible=\"false\">\r\n        <listOfReactants>\r\n          <speciesReference species=\"x\" stoichiometry=\"1\" />\r\n        </listOfReactants>\r\n        <listOfProducts>\r\n          <speciesReference species=\"w\" stoichiometry=\"1\" />\r\n        </listOfProducts>\r\n        <kineticLaw>\r\n          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\r\n            <apply>\r\n              <times />\r\n              <ci> k3 </ci>\r\n              <ci> x </ci>\r\n            </apply>\r\n          </math>\r\n        </kineticLaw>\r\n      </reaction>\r\n    </listOfReactions>\r\n  </model>\r\n</sbml>";

static const char* steadystate_sbml =
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<!-- Created by libAntimony version v2.7.0 on 2015-08-04 09:34 with libSBML version 5.11.3. -->\r\n<sbml xmlns=\"http://www.sbml.org/sbml/level3/version1/core\" level=\"3\" version=\"1\">\r\n  <model id=\"__main\" name=\"__main\">\r\n    <listOfCompartments>\r\n      <compartment sboTerm=\"SBO:0000410\" id=\"default_compartment\" spatialDimensions=\"3\" size=\"1\" constant=\"true\"/>\r\n    </listOfCompartments>\r\n    <listOfSpecies>\r\n      <species id=\"Node1\" compartment=\"default_compartment\" initialConcentration=\"0\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\r\n      <species id=\"Node2\" compartment=\"default_compartment\" initialConcentration=\"0\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\r\n      <species id=\"Node0\" compartment=\"default_compartment\" initialConcentration=\"10\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"true\" constant=\"false\"/>\r\n      <species id=\"Node3\" compartment=\"default_compartment\" initialConcentration=\"0\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"true\" constant=\"false\"/>\r\n    </listOfSpecies>\r\n    <listOfParameters>\r\n      <parameter id=\"J0_Vmax\" value=\"1\" constant=\"true\"/>\r\n      <parameter id=\"J0_Km1\" value=\"0.4\" constant=\"true\"/>\r\n      <parameter id=\"J0_Keq\" value=\"1\" constant=\"true\"/>\r\n      <parameter id=\"J0_Km2\" value=\"1\" constant=\"true\"/>\r\n      <parameter id=\"J1_Vmax\" value=\"1\" constant=\"true\"/>\r\n      <parameter id=\"J1_Km1\" value=\"0.8\" constant=\"true\"/>\r\n      <parameter id=\"J1_Keq\" value=\"1.5\" constant=\"true\"/>\r\n      <parameter id=\"J1_Km2\" value=\"1\" constant=\"true\"/>\r\n      <parameter id=\"J2_Vmax\" value=\"1\" constant=\"true\"/>\r\n      <parameter id=\"J2_Km1\" value=\"0.232\" constant=\"true\"/>\r\n      <parameter id=\"J2_Keq\" value=\"1.6\" constant=\"true\"/>\r\n      <parameter id=\"J2_Km2\" value=\"1\" constant=\"true\"/>\r\n      <parameter id=\"compartment_\" value=\"1\" constant=\"true\"/>\r\n    </listOfParameters>\r\n    <listOfReactions>\r\n      <reaction id=\"J0\" reversible=\"false\" fast=\"false\">\r\n        <listOfReactants>\r\n          <speciesReference species=\"Node0\" stoichiometry=\"1\" constant=\"true\"/>\r\n        </listOfReactants>\r\n        <listOfProducts>\r\n          <speciesReference species=\"Node1\" stoichiometry=\"1\" constant=\"true\"/>\r\n        </listOfProducts>\r\n        <kineticLaw>\r\n          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\r\n            <apply>\r\n              <divide/>\r\n              <apply>\r\n                <times/>\r\n                <apply>\r\n                  <divide/>\r\n                  <ci> J0_Vmax </ci>\r\n                  <ci> J0_Km1 </ci>\r\n                </apply>\r\n                <apply>\r\n                  <minus/>\r\n                  <ci> Node0 </ci>\r\n                  <apply>\r\n                    <divide/>\r\n                    <ci> Node1 </ci>\r\n                    <ci> J0_Keq </ci>\r\n                  </apply>\r\n                </apply>\r\n              </apply>\r\n              <apply>\r\n                <plus/>\r\n                <cn type=\"integer\"> 1 </cn>\r\n                <apply>\r\n                  <divide/>\r\n                  <ci> Node0 </ci>\r\n                  <ci> J0_Km1 </ci>\r\n                </apply>\r\n                <apply>\r\n                  <divide/>\r\n                  <ci> Node1 </ci>\r\n                  <ci> J0_Km2 </ci>\r\n                </apply>\r\n              </apply>\r\n            </apply>\r\n          </math>\r\n        </kineticLaw>\r\n      </reaction>\r\n      <reaction id=\"J1\" reversible=\"false\" fast=\"false\">\r\n        <listOfReactants>\r\n          <speciesReference species=\"Node1\" stoichiometry=\"1\" constant=\"true\"/>\r\n        </listOfReactants>\r\n        <listOfProducts>\r\n          <speciesReference species=\"Node2\" stoichiometry=\"1\" constant=\"true\"/>\r\n        </listOfProducts>\r\n        <kineticLaw>\r\n          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\r\n            <apply>\r\n              <divide/>\r\n              <apply>\r\n                <times/>\r\n                <apply>\r\n                  <divide/>\r\n                  <ci> J1_Vmax </ci>\r\n                  <ci> J1_Km1 </ci>\r\n                </apply>\r\n                <apply>\r\n                  <minus/>\r\n                  <ci> Node1 </ci>\r\n                  <apply>\r\n                    <divide/>\r\n                    <ci> Node2 </ci>\r\n                    <ci> J1_Keq </ci>\r\n                  </apply>\r\n                </apply>\r\n              </apply>\r\n              <apply>\r\n                <plus/>\r\n                <cn type=\"integer\"> 1 </cn>\r\n                <apply>\r\n                  <divide/>\r\n                  <ci> Node1 </ci>\r\n                  <ci> J1_Km1 </ci>\r\n                </apply>\r\n                <apply>\r\n                  <divide/>\r\n                  <ci> Node2 </ci>\r\n                  <ci> J1_Km2 </ci>\r\n                </apply>\r\n              </apply>\r\n            </apply>\r\n          </math>\r\n        </kineticLaw>\r\n      </reaction>\r\n      <reaction id=\"J2\" reversible=\"false\" fast=\"false\">\r\n        <listOfReactants>\r\n          <speciesReference species=\"Node2\" stoichiometry=\"1\" constant=\"true\"/>\r\n        </listOfReactants>\r\n        <listOfProducts>\r\n          <speciesReference species=\"Node3\" stoichiometry=\"1\" constant=\"true\"/>\r\n        </listOfProducts>\r\n        <kineticLaw>\r\n          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\r\n            <apply>\r\n              <divide/>\r\n              <apply>\r\n                <times/>\r\n                <apply>\r\n                  <divide/>\r\n                  <ci> J2_Vmax </ci>\r\n                  <ci> J2_Km1 </ci>\r\n                </apply>\r\n                <apply>\r\n                  <minus/>\r\n                  <ci> Node2 </ci>\r\n                  <apply>\r\n                    <divide/>\r\n                    <ci> Node3 </ci>\r\n                    <ci> J2_Keq </ci>\r\n                  </apply>\r\n                </apply>\r\n              </apply>\r\n              <apply>\r\n                <plus/>\r\n                <cn type=\"integer\"> 1 </cn>\r\n                <apply>\r\n                  <divide/>\r\n                  <ci> Node2 </ci>\r\n                  <ci> J2_Km1 </ci>\r\n                </apply>\r\n                <apply>\r\n                  <divide/>\r\n                  <ci> Node3 </ci>\r\n                  <ci> J2_Km2 </ci>\r\n                </apply>\r\n              </apply>\r\n            </apply>\r\n          </math>\r\n        </kineticLaw>\r\n      </reaction>\r\n    </listOfReactions>\r\n  </model>\r\n</sbml>\r\n";

int main(int argc, char* argv[])
{
    int n;
    // enable logging
//     setLogLevel("debug");
    enableLoggingToFile();
    {
        char* t = getLogFileName();
        fprintf(stderr,"Enabling logging to %s\n", t);
        freeCText(t);
    }

    // print version
    {
        char* t = getVersionStr();
        fprintf(stderr,"RoadRunner version %s\n", t);
        freeCText(t);
    }

    fprintf(stderr,"Initializing RoadRunner...\n");
    RRHandle _handle = createRRInstance();

    {
        char* t = getCurrentIntegratorName(_handle);
        fprintf(stderr, "  Default integrator is %s\n", t);
        freeCText(t);
    }

    for(n = 0; n<1; ++n) {
        double ss_deviation = 0.;
        int k;

        // load the model
        {
            const char* modelstr = steadystate_sbml;
            const char* modeldsc = "steadystate";
            fprintf(stderr,"\n  **** Loading model '%s'\n\n", modeldsc);
            loadSBML(_handle, modelstr);
        }


        fprintf(stderr,"\n  **** Calculating Steady State ****\n\n");


        // find steady state
        steadyState(_handle, &ss_deviation);

        fprintf(stderr, "Steady state reached to within %e\n", ss_deviation);

        // print the floating species ids
        {
            RRStringArrayPtr floatingspcids = getFloatingSpeciesIds(_handle);

            for(k=0; k<floatingspcids->Count; ++k) {
                fprintf(stderr, "%s ", floatingspcids->String[k]);
            }
            fprintf(stderr, "\n");
            freeStringArray(floatingspcids);
        }

        // print the floating species values at steady state
        {
            RRVectorPtr floatingspc = getFloatingSpeciesConcentrations(_handle);

            for(k=0; k<floatingspc->Count; ++k) {
                fprintf(stderr, "%lf ", floatingspc->Data[k]);
            }
            fprintf(stderr, "\n");
            freeVector(floatingspc);
        }
    }

    freeRRInstance(_handle);
	
    return 0;
}



