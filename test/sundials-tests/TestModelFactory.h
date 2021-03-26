#ifndef ROADRUNNER_TESTMODELFACTORY
#define ROADRUNNER_TESTMODELFACTORY

#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <memory>
#include "Variant.h"

//using DoublePair = std::pair<double, double>; // old swig no handle using statements
typedef std::pair<double, double> DoublePair ;

/**
 * Data structure for storing reference simulation results.
 * std::string: model species name
 * std::pair<double, double>: mapping between starting value and simulation result.
 */
typedef std::unordered_map<std::string, DoublePair> InitialToEndResultMap ;

/**
 * Stores component name to expected model value mapping
 */
typedef std::unordered_map<std::string, double> ResultMap ;


/**
 * A collection of ResultMap objects
 * for testing models from multiple starting states.
 */
//using MultiResultsMap = std::vector<ResultMap>;
typedef std::vector<ResultMap> MultiResultsMap ;

/**
 * Abstract type to store sbml string
 */
class TestModel {
public:

    /**
     * Returns the sbml string for this model
     */
    virtual std::string str() = 0;

    /**
     * Returns the model name. This is used by TestModelFactory
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
public:
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
     * todo make this return a full matrix
     *
     */
    virtual InitialToEndResultMap stateVectorAtT10() = 0;
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
class SimpleFlux : public TestModel, public TimeSeriesResult, public SteadyStateResult {
public:

    std::string str() override ;

    std::string modelName() override ;

    InitialToEndResultMap stateVectorAtT10() override;

    ResultMap steadyState() override;

    std::unordered_map<std::string, rr::Variant> settings() override;
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
class SimpleFluxManuallyReduced : public TestModel, public SteadyStateResult {
public:
    std::string str() override;

    std::string modelName() override ;

    ResultMap steadyState() override ;

    std::unordered_map<std::string, rr::Variant> settings() override ;

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
class OpenLinearFlux : public TestModel, public SteadyStateResult, public TimeSeriesResult {
public:
    std::string str() override ;

    ResultMap steadyState() override ;

    InitialToEndResultMap stateVectorAtT10() override ;

    std::string modelName() override ;

    std::unordered_map<std::string, rr::Variant> settings() override ;

};

/**
 * model 269 from the sbml test suite
 */
class Model269 : public TestModel, public TimeSeriesResult {
public:

    std::string str() override ;

    std::string modelName() override ;

    InitialToEndResultMap stateVectorAtT10() override ;

    std::unordered_map<std::string, rr::Variant> settings() override;

};

/**
 * model 28 from the sbml test suite
 */
class Model28 : public TestModel, public TimeSeriesResult {
public:

    std::string str() override;

    std::string modelName() override ;

    InitialToEndResultMap stateVectorAtT10() override;

    std::unordered_map<std::string, rr::Variant> settings() override;

};


/**
 * A model that uses "ceil" in the rate law
 */
class CeilInRateLaw : public TestModel, public TimeSeriesResult {
public:

    std::string str() override ;

    std::string modelName() override ;

    InitialToEndResultMap stateVectorAtT10() override ;

    std::unordered_map<std::string, rr::Variant> settings() override ;

};

/**
 * A model that uses "Factorial" in the rate law
 */
class FactorialInRateLaw : public TestModel, public TimeSeriesResult {
public:

    std::string str() override ;

    std::string modelName() override ;

    InitialToEndResultMap stateVectorAtT10() override;

    std::unordered_map<std::string, rr::Variant> settings() override ;

};

/**
 * Model from the Venkatraman 2010 paper
 */
class Venkatraman2010 : public TestModel, public SteadyStateResult {
public:

    std::string str() override;

    std::string modelName() override ;

    ResultMap steadyState() override ;

    std::unordered_map<std::string, rr::Variant> settings() override;

};

class Brown2004 : public TestModel, public SteadyStateResult {

public:

    std::string str() override ;

    std::string modelName() override ;

    ResultMap steadyState() override ;

    std::unordered_map<std::string, rr::Variant> settings() override ;

};

TestModel* TestModelFactory(const std::string &modelName);


namespace privateSwigTests {
    // this section exists only to test the swig bindings
    // and make sure the typemaps are doing what they are supposed
    // to be. Users should completely ignore this
    //
    // These swig tests are left in for future developers (including
    // my future self) as they will serve as a set of swig examples (CW)
    //

    DoublePair *_testDoublePair(double first, double second);

    std::unordered_map<double, double> *_testDoubleMap(double first, double second) ;

    std::unordered_map<std::string, rr::Variant> *_testVariantMap() ;

    rr::Variant *_testVariant() ;

    InitialToEndResultMap _testResultMap();
}



#endif // ROADRUNNER_TESTMODELFACTORY






