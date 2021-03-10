#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <memory>

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
 * @brief base class interface to result types
 * Defines a settings map for models to 
 * to implement the settings required for 
 * solving the task at hand
 */
class Result {

    virtual std::unordered_map<std::string, rr::Variant> settings() = 0;

};

/**
 * Interface for time series result.
 * Models that implement this interface
 * are models to be used in testing time integrators
 */
class TimeSeriesResult : public Result {
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
class SteadyStateResult : public Result {
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
class SteadyStateMultiStart : public Result {
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
                        {"S1", DoublePair(10, 3.99584)},
                        {"S2", DoublePair(1, 7.00416)}
                };
    }

    ResultMap steadyState() override {
        return ResultMap(
                {
                        {"S1", DoublePair(10, 1.0)},
                        {"S2", DoublePair(1, 10.0)}
                }
        );
    }

    std::unordered_map<std::string, rr::Variant> settings() override {
        return std::unordered_map<std::string, rr::Variant>();
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

    std::unordered_map<std::string, rr::Variant> settings() override {
        return std::unordered_map<std::string, rr::Variant>();
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
               "      <parameter id=\"kout\" value=\"0.2\" constant=\"true\"/>\n"
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
        return ResultMap({
                                 {"S1", DoublePair(0, 10)},
                                 {"S2", DoublePair(0, 5)},
                         });
    }

    std::string modelName() override {
        return "OpenLinearFlux";
    }

    std::unordered_map<std::string, rr::Variant> settings() override {
        return std::unordered_map<std::string, rr::Variant>{
                {"allow_presimulation", true},
                {"presimulation_time",  1}
        };
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

    std::unordered_map<std::string, rr::Variant> settings() override {
        return std::unordered_map<std::string, rr::Variant>();
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

    std::unordered_map<std::string, rr::Variant> settings() override {
        return std::unordered_map<std::string, rr::Variant>();
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

    std::unordered_map<std::string, rr::Variant> settings() override {
        return std::unordered_map<std::string, rr::Variant>();
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

    std::unordered_map<std::string, rr::Variant> settings() override {
        return std::unordered_map<std::string, rr::Variant>();
    }

};

/**
 * Model from the Venkatraman 2010 paper
 */
class Venkatraman2010 : public SBMLTestModel, SteadyStateResult {
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

    ResultMap steadyState() override {
        return ResultMap{
                {"scUPA", DoublePair(10, 0.00010036488071501325)},
                {"PLG",   DoublePair(10, 0.03571790894678159)},
                {"PLS",   DoublePair(10, 0.9642820910532185)},
                {"tcUPA", DoublePair(10, 0.8998996351192852)}
        };
    }

    std::unordered_map<std::string, rr::Variant> settings() override {
        return std::unordered_map<std::string, rr::Variant>();
    }

};

class Brown2004 : public SBMLTestModel, public SteadyStateResult {

public:

    std::string str() override {
        return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
               "<sbml xmlns=\"http://www.sbml.org/sbml/level3/version1/core\" level=\"3\" version=\"1\">\n"
               "  <model metaid=\"Brown2004_NGF_EGF_signaling\" id=\"Brown2004_NGF_EGF_signaling\">\n"
               "    <listOfCompartments>\n"
               "      <compartment id=\"cell\" spatialDimensions=\"3\" size=\"1\" constant=\"true\"/>\n"
               "    </listOfCompartments>\n"
               "    <listOfSpecies>\n"
               "      <species id=\"EGF\" compartment=\"cell\" initialConcentration=\"10002000\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
               "      <species id=\"NGF\" compartment=\"cell\" initialConcentration=\"456000\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
               "      <species id=\"freeEGFReceptor\" compartment=\"cell\" initialConcentration=\"80000\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
               "      <species id=\"boundEGFReceptor\" compartment=\"cell\" initialConcentration=\"0\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
               "      <species id=\"freeNGFReceptor\" compartment=\"cell\" initialConcentration=\"10000\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
               "      <species id=\"boundNGFReceptor\" compartment=\"cell\" initialConcentration=\"0\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
               "      <species id=\"SosInactive\" compartment=\"cell\" initialConcentration=\"120000\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
               "      <species id=\"SosActive\" compartment=\"cell\" initialConcentration=\"0\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
               "      <species id=\"P90RskInactive\" compartment=\"cell\" initialConcentration=\"120000\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
               "      <species id=\"P90RskActive\" compartment=\"cell\" initialConcentration=\"0\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
               "      <species id=\"RasInactive\" compartment=\"cell\" initialConcentration=\"120000\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
               "      <species id=\"RasActive\" compartment=\"cell\" initialConcentration=\"0\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
               "      <species id=\"RasGapActive\" compartment=\"cell\" initialConcentration=\"120000\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"true\" constant=\"false\"/>\n"
               "      <species id=\"Raf1Inactive\" compartment=\"cell\" initialConcentration=\"120000\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
               "      <species id=\"Raf1Active\" compartment=\"cell\" initialConcentration=\"0\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
               "      <species id=\"BRafInactive\" compartment=\"cell\" initialConcentration=\"120000\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
               "      <species id=\"BRafActive\" compartment=\"cell\" initialConcentration=\"0\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
               "      <species id=\"MekInactive\" compartment=\"cell\" initialConcentration=\"600000\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
               "      <species id=\"MekActive\" compartment=\"cell\" initialConcentration=\"0\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
               "      <species id=\"ErkInactive\" compartment=\"cell\" initialConcentration=\"600000\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
               "      <species id=\"ErkActive\" compartment=\"cell\" initialConcentration=\"0\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
               "      <species id=\"PI3KInactive\" compartment=\"cell\" initialConcentration=\"120000\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
               "      <species id=\"PI3KActive\" compartment=\"cell\" initialConcentration=\"0\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
               "      <species id=\"AktInactive\" compartment=\"cell\" initialConcentration=\"120000\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
               "      <species id=\"AktActive\" compartment=\"cell\" initialConcentration=\"0\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
               "      <species id=\"C3GInactive\" compartment=\"cell\" initialConcentration=\"120000\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
               "      <species id=\"C3GActive\" compartment=\"cell\" initialConcentration=\"0\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
               "      <species id=\"Rap1Inactive\" compartment=\"cell\" initialConcentration=\"120000\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
               "      <species id=\"Rap1Active\" compartment=\"cell\" initialConcentration=\"0\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
               "      <species id=\"RapGapActive\" compartment=\"cell\" initialConcentration=\"120000\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"true\" constant=\"false\"/>\n"
               "      <species id=\"PP2AActive\" compartment=\"cell\" initialConcentration=\"120000\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"true\" constant=\"false\"/>\n"
               "      <species id=\"Raf1PPtase\" compartment=\"cell\" initialConcentration=\"120000\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"true\" constant=\"false\"/>\n"
               "    </listOfSpecies>\n"
               "    <listOfParameters>\n"
               "      <parameter id=\"krbEGF\" value=\"2.18503e-05\" constant=\"true\"/>\n"
               "      <parameter id=\"kruEGF\" value=\"0.0121008\" constant=\"true\"/>\n"
               "      <parameter id=\"krbNGF\" value=\"1.38209e-07\" constant=\"true\"/>\n"
               "      <parameter id=\"kruNGF\" value=\"0.00723811\" constant=\"true\"/>\n"
               "      <parameter id=\"kEGF\" value=\"694.731\" constant=\"true\"/>\n"
               "      <parameter id=\"KmEGF\" value=\"6086070\" constant=\"true\"/>\n"
               "      <parameter id=\"kNGF\" value=\"389.428\" constant=\"true\"/>\n"
               "      <parameter id=\"KmNGF\" value=\"2112.66\" constant=\"true\"/>\n"
               "      <parameter id=\"kdSos\" value=\"1611.97\" constant=\"true\"/>\n"
               "      <parameter id=\"KmdSos\" value=\"896896\" constant=\"true\"/>\n"
               "      <parameter id=\"kSos\" value=\"32.344\" constant=\"true\"/>\n"
               "      <parameter id=\"KmSos\" value=\"35954.3\" constant=\"true\"/>\n"
               "      <parameter id=\"kRasGap\" value=\"1509.36\" constant=\"true\"/>\n"
               "      <parameter id=\"KmRasGap\" value=\"1432410\" constant=\"true\"/>\n"
               "      <parameter id=\"kRasToRaf1\" value=\"0.884096\" constant=\"true\"/>\n"
               "      <parameter id=\"KmRasToRaf1\" value=\"62464.6\" constant=\"true\"/>\n"
               "      <parameter id=\"kpRaf1\" value=\"185.759\" constant=\"true\"/>\n"
               "      <parameter id=\"KmpRaf1\" value=\"4768350\" constant=\"true\"/>\n"
               "      <parameter id=\"kpBRaf\" value=\"125.089\" constant=\"true\"/>\n"
               "      <parameter id=\"KmpBRaf\" value=\"157948\" constant=\"true\"/>\n"
               "      <parameter id=\"kpMekCytoplasmic\" value=\"9.85367\" constant=\"true\"/>\n"
               "      <parameter id=\"KmpMekCytoplasmic\" value=\"1007340\" constant=\"true\"/>\n"
               "      <parameter id=\"kdMek\" value=\"2.83243\" constant=\"true\"/>\n"
               "      <parameter id=\"KmdMek\" value=\"518753\" constant=\"true\"/>\n"
               "      <parameter id=\"kdErk\" value=\"8.8912\" constant=\"true\"/>\n"
               "      <parameter id=\"KmdErk\" value=\"3496490\" constant=\"true\"/>\n"
               "      <parameter id=\"kdRaf1\" value=\"0.126329\" constant=\"true\"/>\n"
               "      <parameter id=\"KmdRaf1\" value=\"1061.71\" constant=\"true\"/>\n"
               "      <parameter id=\"kpP90Rsk\" value=\"0.0213697\" constant=\"true\"/>\n"
               "      <parameter id=\"KmpP90Rsk\" value=\"763523\" constant=\"true\"/>\n"
               "      <parameter id=\"kPI3K\" value=\"10.6737\" constant=\"true\"/>\n"
               "      <parameter id=\"KmPI3K\" value=\"184912\" constant=\"true\"/>\n"
               "      <parameter id=\"kPI3KRas\" value=\"0.0771067\" constant=\"true\"/>\n"
               "      <parameter id=\"KmPI3KRas\" value=\"272056\" constant=\"true\"/>\n"
               "      <parameter id=\"kAkt\" value=\"0.0566279\" constant=\"true\"/>\n"
               "      <parameter id=\"KmAkt\" value=\"653951\" constant=\"true\"/>\n"
               "      <parameter id=\"kdRaf1ByAkt\" value=\"15.1212\" constant=\"true\"/>\n"
               "      <parameter id=\"KmRaf1ByAkt\" value=\"119355\" constant=\"true\"/>\n"
               "      <parameter id=\"kC3GNGF\" value=\"146.912\" constant=\"true\"/>\n"
               "      <parameter id=\"KmC3GNGF\" value=\"12876.2\" constant=\"true\"/>\n"
               "      <parameter id=\"kC3G\" value=\"1.40145\" constant=\"true\"/>\n"
               "      <parameter id=\"KmC3G\" value=\"10965.6\" constant=\"true\"/>\n"
               "      <parameter id=\"kRapGap\" value=\"27.265\" constant=\"true\"/>\n"
               "      <parameter id=\"KmRapGap\" value=\"295990\" constant=\"true\"/>\n"
               "      <parameter id=\"kRap1ToBRaf\" value=\"2.20995\" constant=\"true\"/>\n"
               "      <parameter id=\"KmRap1ToBRaf\" value=\"1025460\" constant=\"true\"/>\n"
               "      <parameter id=\"kdBRaf\" value=\"441.287\" constant=\"true\"/>\n"
               "      <parameter id=\"KmdBRaf\" value=\"10879500\" constant=\"true\"/>\n"
               "    </listOfParameters>\n"
               "    <listOfReactions>\n"
               "      <reaction id=\"EGFBindingReaction\" reversible=\"false\" fast=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"EGF\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "          <speciesReference species=\"freeEGFReceptor\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfReactants>\n"
               "        <listOfProducts>\n"
               "          <speciesReference species=\"boundEGFReceptor\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfProducts>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <times/>\n"
               "              <ci> cell </ci>\n"
               "              <ci> krbEGF </ci>\n"
               "              <ci> EGF </ci>\n"
               "              <ci> freeEGFReceptor </ci>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "      <reaction id=\"EGFUnbindingReaction\" reversible=\"false\" fast=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"boundEGFReceptor\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfReactants>\n"
               "        <listOfProducts>\n"
               "          <speciesReference species=\"EGF\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "          <speciesReference species=\"freeEGFReceptor\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfProducts>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <times/>\n"
               "              <ci> cell </ci>\n"
               "              <ci> kruEGF </ci>\n"
               "              <ci> boundEGFReceptor </ci>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "      <reaction id=\"NGFBindingReaction\" reversible=\"false\" fast=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"NGF\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "          <speciesReference species=\"freeNGFReceptor\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfReactants>\n"
               "        <listOfProducts>\n"
               "          <speciesReference species=\"boundNGFReceptor\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfProducts>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <times/>\n"
               "              <ci> krbNGF </ci>\n"
               "              <ci> NGF </ci>\n"
               "              <ci> freeNGFReceptor </ci>\n"
               "              <ci> cell </ci>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "      <reaction id=\"NGFUnbindingReaction\" reversible=\"false\" fast=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"boundNGFReceptor\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfReactants>\n"
               "        <listOfProducts>\n"
               "          <speciesReference species=\"NGF\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "          <speciesReference species=\"freeNGFReceptor\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfProducts>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <times/>\n"
               "              <ci> kruNGF </ci>\n"
               "              <ci> boundNGFReceptor </ci>\n"
               "              <ci> cell </ci>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "      <reaction id=\"SosActivationByEGFReaction\" reversible=\"true\" fast=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"SosInactive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfReactants>\n"
               "        <listOfProducts>\n"
               "          <speciesReference species=\"SosActive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfProducts>\n"
               "        <listOfModifiers>\n"
               "          <modifierSpeciesReference species=\"boundEGFReceptor\"/>\n"
               "        </listOfModifiers>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <times/>\n"
               "              <ci> cell </ci>\n"
               "              <apply>\n"
               "                <divide/>\n"
               "                <apply>\n"
               "                  <times/>\n"
               "                  <ci> kEGF </ci>\n"
               "                  <ci> boundEGFReceptor </ci>\n"
               "                  <ci> SosInactive </ci>\n"
               "                </apply>\n"
               "                <apply>\n"
               "                  <plus/>\n"
               "                  <ci> SosInactive </ci>\n"
               "                  <ci> KmEGF </ci>\n"
               "                </apply>\n"
               "              </apply>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "      <reaction id=\"SosActivationByNGFReaction\" reversible=\"true\" fast=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"SosInactive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfReactants>\n"
               "        <listOfProducts>\n"
               "          <speciesReference species=\"SosActive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfProducts>\n"
               "        <listOfModifiers>\n"
               "          <modifierSpeciesReference species=\"boundNGFReceptor\"/>\n"
               "        </listOfModifiers>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <times/>\n"
               "              <ci> cell </ci>\n"
               "              <apply>\n"
               "                <divide/>\n"
               "                <apply>\n"
               "                  <times/>\n"
               "                  <ci> kNGF </ci>\n"
               "                  <ci> boundNGFReceptor </ci>\n"
               "                  <ci> SosInactive </ci>\n"
               "                </apply>\n"
               "                <apply>\n"
               "                  <plus/>\n"
               "                  <ci> SosInactive </ci>\n"
               "                  <ci> KmNGF </ci>\n"
               "                </apply>\n"
               "              </apply>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "      <reaction id=\"SosDeactivationReaction\" reversible=\"true\" fast=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"SosActive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfReactants>\n"
               "        <listOfProducts>\n"
               "          <speciesReference species=\"SosInactive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfProducts>\n"
               "        <listOfModifiers>\n"
               "          <modifierSpeciesReference species=\"P90RskActive\"/>\n"
               "        </listOfModifiers>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <times/>\n"
               "              <ci> cell </ci>\n"
               "              <apply>\n"
               "                <divide/>\n"
               "                <apply>\n"
               "                  <times/>\n"
               "                  <ci> kdSos </ci>\n"
               "                  <ci> P90RskActive </ci>\n"
               "                  <ci> SosActive </ci>\n"
               "                </apply>\n"
               "                <apply>\n"
               "                  <plus/>\n"
               "                  <ci> SosActive </ci>\n"
               "                  <ci> KmdSos </ci>\n"
               "                </apply>\n"
               "              </apply>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "      <reaction id=\"RasActivationReaction\" reversible=\"true\" fast=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"RasInactive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfReactants>\n"
               "        <listOfProducts>\n"
               "          <speciesReference species=\"RasActive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfProducts>\n"
               "        <listOfModifiers>\n"
               "          <modifierSpeciesReference species=\"SosActive\"/>\n"
               "        </listOfModifiers>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <times/>\n"
               "              <ci> cell </ci>\n"
               "              <apply>\n"
               "                <divide/>\n"
               "                <apply>\n"
               "                  <times/>\n"
               "                  <ci> kSos </ci>\n"
               "                  <ci> SosActive </ci>\n"
               "                  <ci> RasInactive </ci>\n"
               "                </apply>\n"
               "                <apply>\n"
               "                  <plus/>\n"
               "                  <ci> RasInactive </ci>\n"
               "                  <ci> KmSos </ci>\n"
               "                </apply>\n"
               "              </apply>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "      <reaction id=\"RasDeactivationReaction\" reversible=\"true\" fast=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"RasActive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfReactants>\n"
               "        <listOfProducts>\n"
               "          <speciesReference species=\"RasInactive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfProducts>\n"
               "        <listOfModifiers>\n"
               "          <modifierSpeciesReference species=\"RasGapActive\"/>\n"
               "        </listOfModifiers>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <times/>\n"
               "              <ci> cell </ci>\n"
               "              <apply>\n"
               "                <divide/>\n"
               "                <apply>\n"
               "                  <times/>\n"
               "                  <ci> kRasGap </ci>\n"
               "                  <ci> RasGapActive </ci>\n"
               "                  <ci> RasActive </ci>\n"
               "                </apply>\n"
               "                <apply>\n"
               "                  <plus/>\n"
               "                  <ci> RasActive </ci>\n"
               "                  <ci> KmRasGap </ci>\n"
               "                </apply>\n"
               "              </apply>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "      <reaction id=\"Raf1ByRasActivationReaction\" reversible=\"true\" fast=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"Raf1Inactive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfReactants>\n"
               "        <listOfProducts>\n"
               "          <speciesReference species=\"Raf1Active\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfProducts>\n"
               "        <listOfModifiers>\n"
               "          <modifierSpeciesReference species=\"RasActive\"/>\n"
               "        </listOfModifiers>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <times/>\n"
               "              <ci> cell </ci>\n"
               "              <apply>\n"
               "                <divide/>\n"
               "                <apply>\n"
               "                  <times/>\n"
               "                  <ci> kRasToRaf1 </ci>\n"
               "                  <ci> RasActive </ci>\n"
               "                  <ci> Raf1Inactive </ci>\n"
               "                </apply>\n"
               "                <apply>\n"
               "                  <plus/>\n"
               "                  <ci> Raf1Inactive </ci>\n"
               "                  <ci> KmRasToRaf1 </ci>\n"
               "                </apply>\n"
               "              </apply>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "      <reaction id=\"MekbyRaf1ActivationReaction\" reversible=\"true\" fast=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"MekInactive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfReactants>\n"
               "        <listOfProducts>\n"
               "          <speciesReference species=\"MekActive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfProducts>\n"
               "        <listOfModifiers>\n"
               "          <modifierSpeciesReference species=\"Raf1Active\"/>\n"
               "        </listOfModifiers>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <times/>\n"
               "              <ci> cell </ci>\n"
               "              <apply>\n"
               "                <divide/>\n"
               "                <apply>\n"
               "                  <times/>\n"
               "                  <ci> kpRaf1 </ci>\n"
               "                  <ci> Raf1Active </ci>\n"
               "                  <ci> MekInactive </ci>\n"
               "                </apply>\n"
               "                <apply>\n"
               "                  <plus/>\n"
               "                  <ci> MekInactive </ci>\n"
               "                  <ci> KmpRaf1 </ci>\n"
               "                </apply>\n"
               "              </apply>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "      <reaction id=\"MekbyBRafActivationReaction\" reversible=\"true\" fast=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"MekInactive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfReactants>\n"
               "        <listOfProducts>\n"
               "          <speciesReference species=\"MekActive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfProducts>\n"
               "        <listOfModifiers>\n"
               "          <modifierSpeciesReference species=\"BRafActive\"/>\n"
               "        </listOfModifiers>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <times/>\n"
               "              <ci> cell </ci>\n"
               "              <apply>\n"
               "                <divide/>\n"
               "                <apply>\n"
               "                  <times/>\n"
               "                  <ci> kpBRaf </ci>\n"
               "                  <ci> BRafActive </ci>\n"
               "                  <ci> MekInactive </ci>\n"
               "                </apply>\n"
               "                <apply>\n"
               "                  <plus/>\n"
               "                  <ci> MekInactive </ci>\n"
               "                  <ci> KmpBRaf </ci>\n"
               "                </apply>\n"
               "              </apply>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "      <reaction id=\"ErkActivationReaction\" reversible=\"true\" fast=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"ErkInactive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfReactants>\n"
               "        <listOfProducts>\n"
               "          <speciesReference species=\"ErkActive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfProducts>\n"
               "        <listOfModifiers>\n"
               "          <modifierSpeciesReference species=\"MekActive\"/>\n"
               "        </listOfModifiers>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <times/>\n"
               "              <ci> cell </ci>\n"
               "              <apply>\n"
               "                <divide/>\n"
               "                <apply>\n"
               "                  <times/>\n"
               "                  <ci> kpMekCytoplasmic </ci>\n"
               "                  <ci> MekActive </ci>\n"
               "                  <ci> ErkInactive </ci>\n"
               "                </apply>\n"
               "                <apply>\n"
               "                  <plus/>\n"
               "                  <ci> ErkInactive </ci>\n"
               "                  <ci> KmpMekCytoplasmic </ci>\n"
               "                </apply>\n"
               "              </apply>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "      <reaction id=\"MekDeactivationReaction\" reversible=\"true\" fast=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"MekActive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfReactants>\n"
               "        <listOfProducts>\n"
               "          <speciesReference species=\"MekInactive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfProducts>\n"
               "        <listOfModifiers>\n"
               "          <modifierSpeciesReference species=\"PP2AActive\"/>\n"
               "        </listOfModifiers>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <times/>\n"
               "              <ci> cell </ci>\n"
               "              <apply>\n"
               "                <divide/>\n"
               "                <apply>\n"
               "                  <times/>\n"
               "                  <ci> kdMek </ci>\n"
               "                  <ci> PP2AActive </ci>\n"
               "                  <ci> MekActive </ci>\n"
               "                </apply>\n"
               "                <apply>\n"
               "                  <plus/>\n"
               "                  <ci> MekActive </ci>\n"
               "                  <ci> KmdMek </ci>\n"
               "                </apply>\n"
               "              </apply>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "      <reaction id=\"ErkDeactivationReaction\" reversible=\"true\" fast=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"ErkActive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfReactants>\n"
               "        <listOfProducts>\n"
               "          <speciesReference species=\"ErkInactive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfProducts>\n"
               "        <listOfModifiers>\n"
               "          <modifierSpeciesReference species=\"PP2AActive\"/>\n"
               "        </listOfModifiers>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <times/>\n"
               "              <ci> cell </ci>\n"
               "              <apply>\n"
               "                <divide/>\n"
               "                <apply>\n"
               "                  <times/>\n"
               "                  <ci> kdErk </ci>\n"
               "                  <ci> PP2AActive </ci>\n"
               "                  <ci> ErkActive </ci>\n"
               "                </apply>\n"
               "                <apply>\n"
               "                  <plus/>\n"
               "                  <ci> ErkActive </ci>\n"
               "                  <ci> KmdErk </ci>\n"
               "                </apply>\n"
               "              </apply>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "      <reaction id=\"Raf1byPPtaseDeactivationReaction\" reversible=\"true\" fast=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"Raf1Active\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfReactants>\n"
               "        <listOfProducts>\n"
               "          <speciesReference species=\"Raf1Inactive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfProducts>\n"
               "        <listOfModifiers>\n"
               "          <modifierSpeciesReference species=\"Raf1PPtase\"/>\n"
               "        </listOfModifiers>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <times/>\n"
               "              <ci> cell </ci>\n"
               "              <apply>\n"
               "                <divide/>\n"
               "                <apply>\n"
               "                  <times/>\n"
               "                  <ci> kdRaf1 </ci>\n"
               "                  <ci> Raf1PPtase </ci>\n"
               "                  <ci> Raf1Active </ci>\n"
               "                </apply>\n"
               "                <apply>\n"
               "                  <plus/>\n"
               "                  <ci> Raf1Active </ci>\n"
               "                  <ci> KmdRaf1 </ci>\n"
               "                </apply>\n"
               "              </apply>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "      <reaction id=\"P90RskActivationReaction\" reversible=\"true\" fast=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"P90RskInactive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfReactants>\n"
               "        <listOfProducts>\n"
               "          <speciesReference species=\"P90RskActive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfProducts>\n"
               "        <listOfModifiers>\n"
               "          <modifierSpeciesReference species=\"ErkActive\"/>\n"
               "        </listOfModifiers>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <times/>\n"
               "              <ci> cell </ci>\n"
               "              <apply>\n"
               "                <divide/>\n"
               "                <apply>\n"
               "                  <times/>\n"
               "                  <ci> kpP90Rsk </ci>\n"
               "                  <ci> ErkActive </ci>\n"
               "                  <ci> P90RskInactive </ci>\n"
               "                </apply>\n"
               "                <apply>\n"
               "                  <plus/>\n"
               "                  <ci> P90RskInactive </ci>\n"
               "                  <ci> KmpP90Rsk </ci>\n"
               "                </apply>\n"
               "              </apply>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "      <reaction id=\"PI3KbyEGFRActivationReaction\" reversible=\"true\" fast=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"PI3KInactive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfReactants>\n"
               "        <listOfProducts>\n"
               "          <speciesReference species=\"PI3KActive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfProducts>\n"
               "        <listOfModifiers>\n"
               "          <modifierSpeciesReference species=\"boundEGFReceptor\"/>\n"
               "        </listOfModifiers>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <times/>\n"
               "              <ci> cell </ci>\n"
               "              <apply>\n"
               "                <divide/>\n"
               "                <apply>\n"
               "                  <times/>\n"
               "                  <ci> kPI3K </ci>\n"
               "                  <ci> boundEGFReceptor </ci>\n"
               "                  <ci> PI3KInactive </ci>\n"
               "                </apply>\n"
               "                <apply>\n"
               "                  <plus/>\n"
               "                  <ci> PI3KInactive </ci>\n"
               "                  <ci> KmPI3K </ci>\n"
               "                </apply>\n"
               "              </apply>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "      <reaction id=\"PI3KbyRasActivationReaction\" reversible=\"true\" fast=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"PI3KInactive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfReactants>\n"
               "        <listOfProducts>\n"
               "          <speciesReference species=\"PI3KActive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfProducts>\n"
               "        <listOfModifiers>\n"
               "          <modifierSpeciesReference species=\"RasActive\"/>\n"
               "        </listOfModifiers>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <times/>\n"
               "              <ci> cell </ci>\n"
               "              <apply>\n"
               "                <divide/>\n"
               "                <apply>\n"
               "                  <times/>\n"
               "                  <ci> kPI3KRas </ci>\n"
               "                  <ci> RasActive </ci>\n"
               "                  <ci> PI3KInactive </ci>\n"
               "                </apply>\n"
               "                <apply>\n"
               "                  <plus/>\n"
               "                  <ci> PI3KInactive </ci>\n"
               "                  <ci> KmPI3KRas </ci>\n"
               "                </apply>\n"
               "              </apply>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "      <reaction id=\"AktActivationReaction\" reversible=\"true\" fast=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"AktInactive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfReactants>\n"
               "        <listOfProducts>\n"
               "          <speciesReference species=\"AktActive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfProducts>\n"
               "        <listOfModifiers>\n"
               "          <modifierSpeciesReference species=\"PI3KActive\"/>\n"
               "        </listOfModifiers>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <times/>\n"
               "              <ci> cell </ci>\n"
               "              <apply>\n"
               "                <divide/>\n"
               "                <apply>\n"
               "                  <times/>\n"
               "                  <ci> kAkt </ci>\n"
               "                  <ci> PI3KActive </ci>\n"
               "                  <ci> AktInactive </ci>\n"
               "                </apply>\n"
               "                <apply>\n"
               "                  <plus/>\n"
               "                  <ci> AktInactive </ci>\n"
               "                  <ci> KmAkt </ci>\n"
               "                </apply>\n"
               "              </apply>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "      <reaction id=\"Raf1ByAktDeactivationReaction\" reversible=\"true\" fast=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"Raf1Active\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfReactants>\n"
               "        <listOfProducts>\n"
               "          <speciesReference species=\"Raf1Inactive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfProducts>\n"
               "        <listOfModifiers>\n"
               "          <modifierSpeciesReference species=\"AktActive\"/>\n"
               "        </listOfModifiers>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <times/>\n"
               "              <ci> cell </ci>\n"
               "              <apply>\n"
               "                <divide/>\n"
               "                <apply>\n"
               "                  <times/>\n"
               "                  <ci> kdRaf1ByAkt </ci>\n"
               "                  <ci> AktActive </ci>\n"
               "                  <ci> Raf1Active </ci>\n"
               "                </apply>\n"
               "                <apply>\n"
               "                  <plus/>\n"
               "                  <ci> Raf1Active </ci>\n"
               "                  <ci> KmRaf1ByAkt </ci>\n"
               "                </apply>\n"
               "              </apply>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "      <reaction id=\"C3GActivationReaction\" reversible=\"true\" fast=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"C3GInactive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfReactants>\n"
               "        <listOfProducts>\n"
               "          <speciesReference species=\"C3GActive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfProducts>\n"
               "        <listOfModifiers>\n"
               "          <modifierSpeciesReference species=\"boundNGFReceptor\"/>\n"
               "        </listOfModifiers>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <times/>\n"
               "              <ci> cell </ci>\n"
               "              <apply>\n"
               "                <divide/>\n"
               "                <apply>\n"
               "                  <times/>\n"
               "                  <ci> kC3GNGF </ci>\n"
               "                  <ci> boundNGFReceptor </ci>\n"
               "                  <ci> C3GInactive </ci>\n"
               "                </apply>\n"
               "                <apply>\n"
               "                  <plus/>\n"
               "                  <ci> C3GInactive </ci>\n"
               "                  <ci> KmC3GNGF </ci>\n"
               "                </apply>\n"
               "              </apply>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "      <reaction id=\"Rap1ActivationReaction\" reversible=\"true\" fast=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"Rap1Inactive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfReactants>\n"
               "        <listOfProducts>\n"
               "          <speciesReference species=\"Rap1Active\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfProducts>\n"
               "        <listOfModifiers>\n"
               "          <modifierSpeciesReference species=\"C3GActive\"/>\n"
               "        </listOfModifiers>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <times/>\n"
               "              <ci> cell </ci>\n"
               "              <apply>\n"
               "                <divide/>\n"
               "                <apply>\n"
               "                  <times/>\n"
               "                  <ci> kC3G </ci>\n"
               "                  <ci> C3GActive </ci>\n"
               "                  <ci> Rap1Inactive </ci>\n"
               "                </apply>\n"
               "                <apply>\n"
               "                  <plus/>\n"
               "                  <ci> Rap1Inactive </ci>\n"
               "                  <ci> KmC3G </ci>\n"
               "                </apply>\n"
               "              </apply>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "      <reaction id=\"Rap1DeactivationReaction\" reversible=\"true\" fast=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"Rap1Active\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfReactants>\n"
               "        <listOfProducts>\n"
               "          <speciesReference species=\"Rap1Inactive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfProducts>\n"
               "        <listOfModifiers>\n"
               "          <modifierSpeciesReference species=\"RapGapActive\"/>\n"
               "        </listOfModifiers>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <times/>\n"
               "              <ci> cell </ci>\n"
               "              <apply>\n"
               "                <divide/>\n"
               "                <apply>\n"
               "                  <times/>\n"
               "                  <ci> kRapGap </ci>\n"
               "                  <ci> RapGapActive </ci>\n"
               "                  <ci> Rap1Active </ci>\n"
               "                </apply>\n"
               "                <apply>\n"
               "                  <plus/>\n"
               "                  <ci> Rap1Active </ci>\n"
               "                  <ci> KmRapGap </ci>\n"
               "                </apply>\n"
               "              </apply>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "      <reaction id=\"BRafByRap1ActivationReaction\" reversible=\"true\" fast=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"BRafInactive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfReactants>\n"
               "        <listOfProducts>\n"
               "          <speciesReference species=\"BRafActive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfProducts>\n"
               "        <listOfModifiers>\n"
               "          <modifierSpeciesReference species=\"Rap1Active\"/>\n"
               "        </listOfModifiers>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <times/>\n"
               "              <ci> cell </ci>\n"
               "              <apply>\n"
               "                <divide/>\n"
               "                <apply>\n"
               "                  <times/>\n"
               "                  <ci> kRap1ToBRaf </ci>\n"
               "                  <ci> Rap1Active </ci>\n"
               "                  <ci> BRafInactive </ci>\n"
               "                </apply>\n"
               "                <apply>\n"
               "                  <plus/>\n"
               "                  <ci> BRafInactive </ci>\n"
               "                  <ci> KmRap1ToBRaf </ci>\n"
               "                </apply>\n"
               "              </apply>\n"
               "            </apply>\n"
               "          </math>\n"
               "        </kineticLaw>\n"
               "      </reaction>\n"
               "      <reaction id=\"BRafbyPPtaseDeactivationReaction\" reversible=\"true\" fast=\"false\">\n"
               "        <listOfReactants>\n"
               "          <speciesReference species=\"BRafActive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfReactants>\n"
               "        <listOfProducts>\n"
               "          <speciesReference species=\"BRafInactive\" stoichiometry=\"1\" constant=\"true\"/>\n"
               "        </listOfProducts>\n"
               "        <listOfModifiers>\n"
               "          <modifierSpeciesReference species=\"Raf1PPtase\"/>\n"
               "        </listOfModifiers>\n"
               "        <kineticLaw>\n"
               "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
               "            <apply>\n"
               "              <times/>\n"
               "              <ci> cell </ci>\n"
               "              <apply>\n"
               "                <divide/>\n"
               "                <apply>\n"
               "                  <times/>\n"
               "                  <ci> kdBRaf </ci>\n"
               "                  <ci> Raf1PPtase </ci>\n"
               "                  <ci> BRafActive </ci>\n"
               "                </apply>\n"
               "                <apply>\n"
               "                  <plus/>\n"
               "                  <ci> BRafActive </ci>\n"
               "                  <ci> KmdBRaf </ci>\n"
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
        return "Brown2004";
    }

    ResultMap steadyState() override {
        return ResultMap{
                {"EGF",              DoublePair(10002000, 9922004.46501588)},
                {"NGF",              DoublePair(456000, 447048.6328396933)},
                {"freeEGFReceptor",  DoublePair(80000, 4.465015881563886)},
                {"boundEGFReceptor", DoublePair(0, 79995.53498411844)},
                {"freeNGFReceptor",  DoublePair(10000, 1048.632839693278)},
                {"boundNGFReceptor", DoublePair(0, 8951.367160306723)},
                {"SosInactive",      DoublePair(120000, 99569.9548813399)},
                {"SosActive",        DoublePair(0, 20430.0451186601)},
                {"P90RskInactive",   DoublePair(120000, 0)},
                {"P90RskActive",     DoublePair(0, 120000)},
                {"RasInactive",      DoublePair(120000, 115999.51997363668)},
                {"RasActive",        DoublePair(0, 4000.4800263633224)},
                {"Raf1Inactive",     DoublePair(0, 119918.26132477315)},
                {"Raf1Active",       DoublePair(0, 81.73867522685032)},
                {"BRafInactive",     DoublePair(120000, 119315.8938154474)},
                {"BRafActive",       DoublePair(0, 684.1061845525983)},
                {"MekInactive",      DoublePair(60000, 475871.3129791245)},
                {"MekActive",        DoublePair(0, 124128.68702087551)},
                {"ErkInactive",      DoublePair(60000, 118834.97021061042)},
                {"ErkActive",        DoublePair(0, 481165.02978938946)},
                {"PI3KInactive",     DoublePair(120000, 0)},
                {"PI3KActive",       DoublePair(0, 119999.99999999997)},
                {"AktInactive",      DoublePair(0, 0)},
                {"AktActive",        DoublePair(0, 120000)},
                {"C3GInactive",      DoublePair(120000, 0)},
                {"C3GActive",        DoublePair(0, 120000)},
                {"Rap1Inactive",     DoublePair(120000, 105544.62439152892)},
                {"Rap1Active",       DoublePair(0, 14455.375608471077)}
        };

    };

    std::unordered_map<std::string, rr::Variant> settings() override {
        return std::unordered_map<std::string, rr::Variant>{
                {"allow_presimulation", true},
                {"presimulation_time",  10}
        };
    }


};


/**
 * Basic factory that creates sbml strings
 * for use in tests.
 */
std::unique_ptr<SBMLTestModel> SBMLTestModelFactory(const std::string &modelName) {
    if (modelName == "SimpleFlux") {
        return std::make_unique<SimpleFlux>();
    } else if (modelName == "Model269") {
        return std::make_unique<Model269>();
    } else if (modelName == "Model28") {
        return std::make_unique<Model28>();
    } else if (modelName == "CeilInRateLaw") {
        return std::make_unique<CeilInRateLaw>();
    } else if (modelName == "FactorialInRateLaw") {
        return std::make_unique<FactorialInRateLaw>();
    } else if (modelName == "Venkatraman2010") {
        return std::make_unique<Venkatraman2010>();
    } else if (modelName == "OpenLinearFlux") {
        return std::make_unique<OpenLinearFlux>();
    } else if (modelName == "SimpleFluxManuallyReduced") {
        return std::make_unique<SimpleFluxManuallyReduced>();
    } else if (modelName == "Brown2004") {
        return std::make_unique<Brown2004>();
    } else {
        throw std::runtime_error(
                "SBMLTestModelFactory::SBMLTestModelFactory(): no model called \"" + modelName + "\" found\n");
    }
}














