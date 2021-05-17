#ifndef ROADRUNNER_TESTMODELFACTORY
#define ROADRUNNER_TESTMODELFACTORY

#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <memory>
#include "Setting.h"
#include "rr-libstruct/lsMatrix.h"

//using DoublePair = std::pair<double, double>; // old swig no handle using statements
typedef std::pair<double, double> DoublePair;

/**
 * Data structure for storing reference simulation results.
 * std::string: model species name
 * std::pair<double, double>: mapping between starting value and simulation result.
 */
typedef std::unordered_map<std::string, DoublePair> StringDoublePairMap;

/**
 * Stores component name to expected model value mapping
 */
typedef std::unordered_map<std::string, double> StringDoubleMap;


/**
 * A collection of ResultMap objects
 * for testing models from multiple starting states.
 */
//using MultiResultsMap = std::vector<ResultMap>;
typedef std::vector<StringDoubleMap> VectorStringDoubleMap;

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
 * @brief base class interface to result types.
 * @details exists only for any polymorphism that may
 * be implemented in future.
 */
class Result {
};

/**
 * Interface for time series result.
 * Models that implement this interface
 * are models to be used in testing time integrators
 */
class TimeSeriesResult : public Result {
public:
    /**
     * @brief returns the correct results for a time series simulation.
     * @details These results are always computed using an independent simulator.
     * Options provided in the timeSeriesSettings determine time, tol, duration etc.
     */
    virtual ls::DoubleMatrix timeSeriesResult() = 0;

    virtual std::unordered_map<std::string, rr::Setting> timeSeriesSettings();

};

/**
 * @brief Interface for classes that compute the steady state
 * from single parameter set.
 */
class SteadyStateResult : public Result {
public:

    virtual StringDoubleMap steadyState() = 0;

    /**
     * @brief Settings map for steady state tasks.
     * @details classes that implement this interface do not necessarily
     * need to override this method, since they can use default settings.
     * Empty by default.
     */
    virtual std::unordered_map<std::string, rr::Setting> steadyStateSettings();


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

    virtual VectorStringDoubleMap steadyState() = 0;
};

/**
 * @brief interface for models that test the
 * jacobian matrix.
 * @details classes that implement this interface
 * are expected to hard code the results of the
 * jacobian matrix, calculated with an independent
 * tool (or better - analytically ). Both the full
 * and reduced jacobians should be computed and stored
 * as both amounts and concentrations
 */
class JacobianResult : public Result {
public:
    virtual ls::DoubleMatrix fullJacobianAmt() = 0;

    virtual ls::DoubleMatrix fullJacobianConc() = 0;

    virtual ls::DoubleMatrix reducedJacobianAmt() = 0;

    virtual ls::DoubleMatrix reducedJacobianConc() = 0;

    virtual std::unordered_map<std::string, rr::Setting> jacobianSettings() = 0;
};




namespace privateSwigTests_ {
    // this section exists only to test the swig bindings
    // and make sure the typemaps are doing what they are supposed
    // to. Users should never see this
    //
    // These were originally built as simple examples of how to
    // use swig. However, given the steep learning curve that is
    // swig, instead of deleting they are left as an example
    // to future developers (including my future self).
    //

    DoublePair *_testDoublePair(double first, double second);

    std::unordered_map<double, double> *_testDoubleMap(double first, double second);

    std::unordered_map<std::string, rr::Setting> *_testVariantMap();

    rr::Setting *_testVariant();

    StringDoublePairMap _testResultMap();
}


#endif // ROADRUNNER_TESTMODELFACTORY






