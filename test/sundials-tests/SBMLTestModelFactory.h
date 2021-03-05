#include <vector>
#include <unordered_map>
#include <stdexcept>


using DoublePair = std::pair<double, double>;

/**
 * Data structure for storing reference simulation results.
 * std::string: model species name
 * std::pair<double, double>: mapping between starting value and simulation result.
 */
using ResultMap = std::unordered_map<std::string, DoublePair>;

/**
 * A collection of ResultMap objects
 * for testing models from multiple starting states.
 */
using MultiResultsMap = std::vector<ResultMap>;


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

};

/**
 * Interface for time series result.
 * Models that implement this interface
 * are models to be used in testing time integrators
 */
class TimeSeriesResult {
public:
    /**
     * @brief returns the state of the system at t=10 time units.
     * This method is used for storing the true values of
     * the model simulation at time point 10. These values
     * must not be computed by roadrunner (otherwise what
     * are we testing?). Instead an independent sbml simulator
     * should be used to extract the state vector at t=10.
     *
     * todo turn t=10 into an argument so that we can test
     *  any time step.
     *  ... or better, a full matrix
     *
     */
    virtual ResultMap stateVectorAtT10() = 0;
};

/**
 * @brief Interface for classes that compute the steady state
 * from single parameter set.
 */
class SteadyStateResult {
public:

    virtual ResultMap steadyState() = 0;
};

/**
 * @brief Interface for classes that compute the steady state
 * from multiple parameter sets.
 * @details SteadyStateResult was a bit limiting in that we can
 * only encode a single steady state test result from a single
 * parameter set. This interface addresses this problem.
 */
class SteadyStateMultiStart {
public:

    virtual MultiResultsMap steadyState() = 0;
};


/**
 * A -> B; k1
 * B -> A; k2
 * k1 = 0.5
 * k2 = 1.0
 * A = 10;
 * B = 1;
 */
class SimpleFlux : public SBMLTestModel, TimeSeriesResult, SteadyStateResult {
public:

    std::string str() override {
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

    std::string modelName() override {
        return "SimpleFlux";
    }

    ResultMap stateVectorAtT10() override {
        return ResultMap
                {
                        {"S1", DoublePair(10, 9.090924288825768)},
                        {"S2", DoublePair(1, 0.9090757111742356)}
                };
    }

    ResultMap steadyState() override {
        return ResultMap (
                {
                        {"S1", DoublePair(10, 1.0)},
                        {"S2", DoublePair(1, 10.0)}
                }
        );
    }
};

/**
 * @brief This model is the same as SimpleFlux but the network has been reduced
 * using conservation analysis manually before encoding in sbml.
 *
 * @details The system we solve is:
 *      f(x) =
 *       dS1/dt = - kf*S1 + kb*S2
 *       dS2/dt = + kf*S1 - kb*S2
 * We use
 *  - kf = 0.1
 *  - kb = 0.01
 *  - S1 @t0 = 10
 *  - S2 @t0 = 1
 *  - TOTAL = S1 + S2
 *
 * The solution to f(x) = 0 is:
 *  - S1 = 1
 *  - S2 = 10
 *
 * To get to this solution, we need to do conservation
 * analysis. Otherwise the jacobian matrix is singular,
 * and cannot be inverted (to solve newton iteration).
 * For this, we note that S1 + S2 = TOTAL, where TOTAL
 * is constant. Therefore we can reduce this system of
 * 2 equations into 1 differential and 1 algebraic.
 *
 * The reduced system is:
 *      f(x) =
 *       Total = S1 + S2
 *       S2Conserved = (Total - S1)
 *       dS1/dt = - kf*S1 + kb*S2Conserved
 *
 * The equivalent roadrunner/tellurium code is:
 *
 *  def ss(s):
 *      m = te.loada(s)
 *      m.conservedMoietyAnalysis = True
 *      m.steadyState()
 *      print(m.getFloatingSpeciesConcentrationIds())
 *      print(m.getFloatingSpeciesConcentrations())
 *      print(m.getFullJacobian())
 *      print(m.getReducedJacobian())
 *      print(m.getReducedStoichiometryMatrix())
 *      print(m.getGlobalParameterIds())
 *      print(m.getGlobalParameterValues())
 *      return m
 *  r = ss("""
 *  model x
 *      S1 = 10;
 *      S20 = 1;
 *      Total = S1 + S20;
 *      S1 => ; - kf*S1 + kb*(Total - S1);
 *      S2 := Total - S1;
 *      kf = 0.1;
 *      kb = 0.01;
 *  end
 *  """)
 *  The expected output:
 *      ['[S1]']
 *      [1.]
 *              S1
 *      S1 [[ 0.11]]
 *
 *              S1
 *      S1 [[ 0.11]]
 *
 *            _J0
 *      S1 [[  -1]]
 *
 *      ['S20', 'Total', 'kf', 'kb', 'S2']
 *      [1.0e+00 1.1e+01 1.0e-01 1.0e-02 1.0e+01]
 */
class SimpleFluxManuallyReduced : public SBMLTestModel, SteadyStateResult {
public:
    std::string str() override {
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

    std::string modelName() override {
        return "SimpleFluxManuallyReduced";
    }

    ResultMap steadyState() override {
        return ResultMap({
                            {"S1", DoublePair(10, 1.0)}
                    });
    }
};

/**
 * In [33]:  def ss(s):
    ...:      m = te.loada(s)
    ...:      m.conservedMoietyAnalysis = False
    ...:      m.steadyState()
    ...:      print(m.getFloatingSpeciesConcentrations())
    ...:      print(m.getFloatingSpeciesConcentrationIds())
    ...:      print(m.getFullJacobian())
    ...:      return m
    ...:
    ...:  r = ss("""
    ...:  model x
    ...:      S1 = 0;
    ...:      S2 = 0;
    ...:      => S1; kin
    ...:      S1 => S2; kf*S1;
    ...:      S2 => ; S2*kout;
    ...:      kf = 0.1;
    ...:      kb = 0.01;
    ...:      kin = 1;
    ...:      kout = 0.1
    ...:  end
    ...:  """)
 */
class OpenLinearFlux : public SBMLTestModel, SteadyStateResult {
public:
    std::string str() override {
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
               "      <parameter id=\"kout\" value=\"0.1\" constant=\"true\"/>\n"
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

    ResultMap steadyState() override {
        return ResultMap ({
                {"S1", DoublePair(0, 10)},
                {"S2", DoublePair(0, 10)},
        });
    }

    std::string modelName() override {
        return "OpenLinearFlux";
    }
};

/**
 * model 269 from the sbml test suite
 */
class Model269 : public SBMLTestModel, TimeSeriesResult {
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

    ResultMap stateVectorAtT10() override {
        return ResultMap{
                {"S1", DoublePair(1, 0.0270834)},
                {"S2", DoublePair(1, 0.972917)}
        };
    }
};

/**
 * model 28 from the sbml test suite
 */
class Model28 : public SBMLTestModel, TimeSeriesResult {
public:

    std::string str() override {
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

    std::string modelName() override {
        return "Model28";
    }

    ResultMap stateVectorAtT10() override {
        return ResultMap{
                {"S1", DoublePair(1, 0.0270834)},
                {"S2", DoublePair(1, 0.972917)}
        };
    }
};


/**
 * A model that uses "ceil" in the rate law
 */
class CeilInRateLaw : public SBMLTestModel, TimeSeriesResult {
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

    ResultMap stateVectorAtT10() override {
        return ResultMap{
                {"S1", DoublePair(10, 9.02844e-13)},
                {"S2", DoublePair(10, 10)}
        };
    }
};

/**
 * A model that uses "Factorial" in the rate law
 */
class FactorialInRateLaw : public SBMLTestModel, TimeSeriesResult {
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

    ResultMap stateVectorAtT10() override {
        return ResultMap{
                {"S1", DoublePair(10, 1.46671e-12)},
                {"S2", DoublePair(10, 10)}
        };
    }
};

/**
 * Model from the Venkatraman 2010 paper
 */
class Venkatraman2010 : public SBMLTestModel, SteadyStateMultiStart {
public:

    std::string str() override {
        return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
               "<sbml xmlns=\"http://www.sbml.org/sbml/level3/version1/core\" level=\"3\" version=\"1\">\n"
               "  <model metaid=\"__main\" id=\"__main\">\n"
               "    <listOfCompartments>\n"
               "      <compartment sboTerm=\"SBO:0000410\" id=\"default_compartment\" spatialDimensions=\"3\" size=\"1\" constant=\"true\"/>\n"
               "    </listOfCompartments>\n"
               "    <listOfSpecies>\n"
               "      <species id=\"scUPA\" compartment=\"default_compartment\" initialConcentration=\"1.16213e-8\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
               "      <species id=\"PLG\" compartment=\"default_compartment\" initialConcentration=\"0.0262792\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
               "      <species id=\"PLS\" compartment=\"default_compartment\" initialConcentration=\"19.7847\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
               "      <species id=\"tcUPA\" compartment=\"default_compartment\" initialConcentration=\"19.9809\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
               "    </listOfSpecies>\n"
               "    <listOfParameters>\n"
               "      <parameter id=\"keff1\" value=\"0.0017\" constant=\"true\"/>\n"
               "      <parameter id=\"keff2\" value=\"1\" constant=\"true\"/>\n"
               "      <parameter id=\"keff3\" value=\"0.03\" constant=\"true\"/>\n"
               "      <parameter id=\"n\" value=\"3\" constant=\"true\"/>\n"
               "      <parameter id=\"u1\" value=\"0.0001\" constant=\"true\"/>\n"
               "      <parameter id=\"u3\" value=\"0.0001\" constant=\"true\"/>\n"
               "      <parameter id=\"u2\" value=\"0.001\" constant=\"true\"/>\n"
               "      <parameter id=\"u4\" value=\"0.001\" constant=\"true\"/>\n"
               "      <parameter id=\"alpha1\" value=\"9e-05\" constant=\"true\"/>\n"
               "      <parameter id=\"alpha2\" value=\"0.001\" constant=\"true\"/>\n"
               "    </listOfParameters>\n"
               "    <listOfReactions>\n"
               "      <reaction id=\"r1\" reversible=\"true\" fast=\"false\">\n"
               "        <listOfProducts>\n"
               "          <speciesReference species=\"scUPA\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfProducts>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <ci> alpha1 </ci>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "      <reaction id=\"r2\" reversible=\"true\" fast=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"scUPA\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfReactants>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <times/>\n"
               "              <ci> scUPA </ci>\n"
               "              <ci> u1 </ci>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "      <reaction id=\"r3\" reversible=\"true\" fast=\"false\">\n"
               "        <listOfProducts>\n"
               "          <speciesReference species=\"PLG\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfProducts>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <ci> alpha2 </ci>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "      <reaction id=\"r4\" reversible=\"true\" fast=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"PLG\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfReactants>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <times/>\n"
               "              <ci> PLG </ci>\n"
               "              <ci> u2 </ci>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "      <reaction id=\"r5\" reversible=\"true\" fast=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"PLG\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfReactants>\n"
               "        <listOfProducts>\n"
               "          <speciesReference species=\"PLS\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfProducts>\n"
               "        <listOfModifiers>\n"
               "          <modifierSpeciesReference species=\"scUPA\"/>\n"
               "        </listOfModifiers>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <times/>\n"
               "              <ci> keff1 </ci>\n"
               "              <ci> scUPA </ci>\n"
               "              <ci> PLG </ci>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "      <reaction id=\"r6\" reversible=\"true\" fast=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"scUPA\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfReactants>\n"
               "        <listOfProducts>\n"
               "          <speciesReference species=\"tcUPA\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfProducts>\n"
               "        <listOfModifiers>\n"
               "          <modifierSpeciesReference species=\"PLS\"/>\n"
               "        </listOfModifiers>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <times/>\n"
               "              <ci> keff2 </ci>\n"
               "              <ci> scUPA </ci>\n"
               "              <apply>\n"
               "                <power/>\n"
               "                <ci> PLS </ci>\n"
               "                <ci> n </ci>\n"
               "              </apply>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "      <reaction id=\"r7\" reversible=\"true\" fast=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"PLG\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfReactants>\n"
               "        <listOfProducts>\n"
               "          <speciesReference species=\"PLS\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfProducts>\n"
               "        <listOfModifiers>\n"
               "          <modifierSpeciesReference species=\"tcUPA\"/>\n"
               "        </listOfModifiers>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <times/>\n"
               "              <ci> keff3 </ci>\n"
               "              <ci> tcUPA </ci>\n"
               "              <ci> PLG </ci>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "      <reaction id=\"r8\" reversible=\"true\" fast=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"tcUPA\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfReactants>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <times/>\n"
               "              <ci> tcUPA </ci>\n"
               "              <ci> u3 </ci>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "      <reaction id=\"r9\" reversible=\"true\" fast=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"PLS\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfReactants>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <times/>\n"
               "              <ci> PLS </ci>\n"
               "              <ci> u4 </ci>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "    </listOfReactions>\n"
               "  </model>\n"
               "</sbml>";
    }

    std::string modelName() override {
        return "Venkatraman2010";
    }

    MultiResultsMap steadyState() override {
        MultiResultsMap results;
        results.push_back(
                {
                        // This parameter set doesn't solve
                        {"scUPA", DoublePair(10, 0.00010036488071501325)},
                        {"PLG",   DoublePair(10, 0.03571790894678159)},
                        {"PLS",   DoublePair(10, 0.9642820910532185)},
                        {"tcUPA", DoublePair(10, 0.8998996351192852)}
                }
        );
        results.push_back(
                {
                        // this starting set comes from integrating the model to t=10 before solving
                        {"scUPA", DoublePair(1.16213e-8, 0.00010036488071501325)},
                        {"PLG",   DoublePair(0.0262792, 0.03571790894678159)},
                        {"PLS",   DoublePair(19.7847, 0.9642820910532185)},
                        {"tcUPA", DoublePair(19.9809, 0.8998996351192852)}
                }
        );
        return results;
    }
};


/**
 * Basic factory that creates sbml strings
 * for use in tests.
 *
 * The caller is responsible for deleting memory
 * associated with the create SBMLTestModel
 */
SBMLTestModel *SBMLTestModelFactory(const std::string &modelName) {
    if (modelName == "SimpleFlux") {
        return new SimpleFlux();
    } else if (modelName == "Model269") {
        return new Model269();
    } else if (modelName == "Model28") {
        return new Model28();
    } else if (modelName == "CeilInRateLaw") {
        return new CeilInRateLaw();
    } else if (modelName == "FactorialInRateLaw") {
        return new FactorialInRateLaw();
    } else if (modelName == "Venkatraman2010") {
        return new Venkatraman2010();
    } else if (modelName == "OpenLinearFlux") {
        return new OpenLinearFlux();
    } else if (modelName == "SimpleFluxManuallyReduced") {
        return new SimpleFluxManuallyReduced();
    } else {
        throw std::runtime_error(
                "SBMLTestModelFactory::SBMLTestModelFactory(): no model called \"" + modelName + "\" found\n");
    }
}














