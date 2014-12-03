/*
 * Integrator.h
 *
 *  Created on: Sep 7, 2013
 *      Author: andy
 */

#ifndef INTEGRATOR_H_
#define INTEGRATOR_H_

#include "rrLogger.h"
#include "rrOSSpecifics.h"
#include "Dictionary.h"
#include <stdexcept>

#if (__cplusplus >= 201103L) || defined(_MSC_VER)
#include <memory>
#define cxx11_ns std
#else
#include <tr1/memory>
#define cxx11_ns std::tr1
#endif

namespace rr
{

class Integrator;
class ExecutableModel;

/**
 * Listen for integrator events.
 *
 * These are called after the event occured or was processed.
 *
 * The internal integrator typically iterates for many internal time
 * steps, at variable step size for each external time step. So, if
 * RoadRunner::oneStep is called with a step size of say 10 time units, the internal
 * integrator may potentially integrate for 1, 5, 10 100 or some value. The
 * onTimeStep method is called for each of these internal time steps.
 *
 * The return values are currently a place holder and are ignored.
 */
class IntegratorListener
{
public:

    /**
     * is called after the internal integrator completes each internal time step.
     */
    virtual uint onTimeStep(Integrator* integrator, ExecutableModel* model, double time) = 0;

    /**
     * whenever model event occurs and after it is procesed.
     */
    virtual uint onEvent(Integrator* integrator, ExecutableModel* model, double time) = 0;

    virtual ~IntegratorListener() {};
};

/**
 * listeners are shared objects, so use std smart pointers
 * to manage them.
 */
typedef cxx11_ns::shared_ptr<IntegratorListener> IntegratorListenerPtr;

class SimulateOptions;

/**
 * Interface to a class which advances a model forward in time.
 *
 * The Integrator is only valid if attached to a model.
 */
class RR_DECLSPEC Integrator : public Dictionary
{
public:

    /**
     * Set the configuration parameters the integrator uses.
     */
    virtual void setSimulateOptions(const SimulateOptions* options) = 0;

    /**
     * integrates the model from t0 to t0 + hstep
     *
     * @return the final time value. This is typically very close to t0 + hstep,
     * but may be different if variableStep is used.
     */
    virtual double integrate(double t0, double hstep) = 0;

    /**
     * copies the state vector out of the model and into cvode vector,
     * re-initializes cvode.
     */
    virtual void restart(double t0) = 0;

    /**
     * the integrator can hold a single listener. If clients require multicast,
     * they can create a multi-cast listener.
     */
    virtual void setListener(IntegratorListenerPtr) = 0;

    /**
     * get the integrator listener
     */
    virtual IntegratorListenerPtr getListener() = 0;

    /**
     * get a description of this object, compatable with python __str__
     */
    virtual std::string toString() const = 0;

    /**
     * get a short descriptions of this object, compatable with python __repr__.
     */
    virtual std::string toRepr() const = 0;

    /**
     * get the name of this integrator
     */
    virtual std::string getName() const = 0;

    /**
     * this is an interface, provide virtual dtor as instances are
     * returned from New which must be deleted.
     */
    virtual ~Integrator() {};

    /**
     * the list of ODE solvers RoadRunner currently supports.
     *
     * The last item, INTEGRATOR_END needs to always be the last item
     * it is not a valid integrator, just used to indicate how many
     * we have.
     */
    enum IntegratorId
    {
        /**
         * The default CVODE integrator from the Sundials package.
         */
        CVODE = 0,

        /**
         * Basic Gillespie stochastic integrator.
         */
        GILLESPIE,

        /**
         * Basic Runge-Kutta fourth order integrator.
         */
        RK4,

        /**
         * Always has to be at the end, this way, this value indicates
         * how many integrators we have.
         */
        INTEGRATOR_END
    };


    /**
     * Integrators may be either deterministic or stochastic
     */
    enum IntegratorType
    {
        /**
         * Every set of variable states is uniquely determined by parameters
         * in the model and by sets of previous states of these variables.
         * Therefore, deterministic integrators perform the same way for a given
         * set of initial conditions
         */
        DETERMINISTIC,

        /**
         * Randomness is present, and variable states are not described by
         * unique values, but rather by probability distributions.
         */
        STOCHASTIC
    };


    /**
     * A set of flags that are common to all integrators. The SimulateOptions::integratorFlags
     * field is a bitmask that contains these values.
     */
    enum IntegratorFlags
    {
        /**
         * Is the model a stiff system? setting this to stiff causes
         * RoadRunner to load a stiff solver which could potentially be
         * extremly slow
         */
        STIFF                   = (0x1 << 0), // => 0x00000001

        /**
         * The MULTI_STEP option tells the solver to take a series of internal steps
         * and then return the solution at the point reached by that step.
         *
         * In simulate, this option will likely be slower than normal mode,
         * but may be useful to monitor solutions as they are integrated.
         *
         * This is intended to be used in combination with the
         * IntegratorListener. It this option is set, and there is a
         * IntegratorListener set, RoadRunner::integrate will run the
         * integrator in a series of internal steps, and the listner
         * will by notified at each step.
         *
         * Highly Experimental!!!
         */
        MULTI_STEP                = (0x1 << 1), // => 0x00000010

        /**
         * Perform a variable time step simulation. This will allow the
         * integrator to best choose an adaptive time step and the resulting
         * matrix will have a non-uniform time column
         */
        VARIABLE_STEP             = (0x1 << 2) // => 0b00000100
    };
};


class IntegratorException: public std::runtime_error
{
public:
    explicit IntegratorException(const std::string& what) :
            std::runtime_error(what)
    {
        Log(rr::Logger::LOG_ERROR) << __FUNC__ << "what: " << what;
    }

    explicit IntegratorException(const std::string& what, const std::string &where) :
            std::runtime_error(what + "; In " + where)
    {
        Log(rr::Logger::LOG_ERROR) << __FUNC__ << "what: " << what << ", where: " << where;
    }
};

/**
 * A factory class to create an integrator.
 *
 * This class also has a number of static method which return the list of
 * available integrators, and methods to return dictionaries of the available
 * options each Integrator supports.
 *
 * The Dictionary references that are returned are borrowed references, if one
 * wants to modify them, a new SimululateOptions or DictionaryImpl may be created
 * using their copy constructors.
 *
 * When a new mutable dictionary is created, it may be used as the
 * argument to RoadRunner::simulate();
 *
 * The integrator dictionary contain a set of descriptor keys / values, these
 * are the standard key names, but have a ".description" or ".hint" suffix.
 *
 * For example, to iterate through all of the available integrator options,
 * and display them to the console, one would:
 * @code
 *     // get a list of all of the integrator options
 *   vector<const Dictionary*> opts =
 *           IntegratorFactory::getIntegratorOptions();
 *
 *   // iterate through the integrator options
 *   for(int i = 0; i < opts.size(); ++i) {
 *       // each dictionary will contain all the keys that
 *       // are valid for a particular integrator.
 *       const Dictionary &d = *opts[i];
 *       vector<string> keys = d.getKeys();
 *       for(int j = 0; j < keys.size(); ++j) {
 *           string key = keys[j];
 *           string item = d.getItem(key);
 *           cout << "key: " << key << ", value: " << item << std::endl;
 *       }
 *   }
 * @endcode
 *
 * Notice here, the line, `string item = d.getItem(key)`, this implicitly calls
 * the conversion operator to automatically convert the contents of the Variant to
 * a string. Any type that is stored in a Variant can be converted to a string.
 *
 * Each Dictionary object will contain a "integrator" key, this identifies the
 * integrator. It will also contain a "integrator.description" and an "integrator.hint"
 * keys, these provide a full description and a hint for that particular integrator.
 *
 * One may also get a list of all the available integrators via the
 * getIntegratorNames() method, and use this integrator name to get the
 * list of options available for a particular integrator.
 *
 * The values in each integrator options dictionary are those of the current
 * default values. The default values may be modified via the Config class.
 */
class RR_DECLSPEC IntegratorFactory
{
public:
    /**
     * create a new integrator based on the settings in the
     * options class.
     *
     * The new integrator borrows a reference to an ExecutableModel object.
     *
     * @param opt: a pointer to a Dictionary object whose values will be used
     * to construct a new Integrator.
     * @param m: a borrowed reference to an ExecutableModel object, the new
     * Integrator will be attached to this object. This value may also be NULL,
     * in which the new Integrator is not attached to any model.
     * @returns a newly created Integrator. The caller is responsible for
     * deleting this object.
     */
    static Integrator* New(const Dictionary *opt, ExecutableModel *m);

    /**
     * The list of integrator names that are currently implemented.
     */
    static std::vector<std::string> getIntegratorNames();

    /**
     * list of options that this integrator supports.
     *
     * Each dictionary is populated with the default values that
     * the integrator will be created with.
     *
     * @returns a list of borrowed Dictionary references.
     */
    static std::vector<const Dictionary*> getIntegratorOptions();

    /**
     * Get a dictionary of options for a specific integrator.
     *
     * @param intName: the name of a valid integrator
     * @returns a borrowed reference to a Dictionary
     * @throws std::exception if the argument is not a valid integrator.
     */
    static const Dictionary* getIntegratorOptions(const std::string& intName);

    /**
     * @internal
     * get the type of integrator.
     */
    static Integrator::IntegratorType getIntegratorType(Integrator::IntegratorId i);

    /**
     * @internal
     * get the textual name of the integrator.
     */
    static std::string getIntegratorNameFromId(Integrator::IntegratorId);

    /**
     * @internal
     * mape the textual name of an integrator to its
     * enumerated id.
     */
    static Integrator::IntegratorId getIntegratorIdFromName(const std::string& name);
};

}

#endif /* INTEGRATOR_H_ */
