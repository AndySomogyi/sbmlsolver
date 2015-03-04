#ifndef rrCvodeInterfaceH
#define rrCvodeInterfaceH

#include <SBMLSolverOptions.h>
#include "Integrator.h"
#include <string>
#include <vector>

/**
 * CVode vector struct
 */
typedef struct _generic_N_Vector *N_Vector;

namespace rr
{

using std::string;

class ExecutableModel;
class SBMLSolver;

/**
 * @internal
 * The integrator implemented by CVODE.
 */
class CVODEIntegrator : public Integrator
{
public:
    CVODEIntegrator(ExecutableModel* oModel, const SimulateOptions* options);

    virtual ~CVODEIntegrator();

    double integrate(double t0, double tf);

    /**
     * copies the state vector out of the model and into cvode vector,
     * re-initializes cvode.
     */
    void restart(double timeStart);

    /**
     * set the options the integrator will use.
     */
    virtual void setSimulateOptions(const SimulateOptions* options);

    /**
     * the integrator can hold a single listener. If clients require multicast,
     * they can create a multi-cast listener.
     */
    virtual void setListener(IntegratorListenerPtr);

    /**
     * get the integrator listener
     */
    virtual IntegratorListenerPtr getListener();

    /**
     * implement dictionary interface
     */
    virtual void setItem(const std::string& key, const rr::Variant& value);

    virtual Variant getItem(const std::string& key) const;

    virtual bool hasKey(const std::string& key) const;

    virtual int deleteItem(const std::string& key);

    virtual std::vector<std::string> getKeys() const;

    /**
     * get a description of this object, compatable with python __str__
     */
    virtual std::string toString() const;

    /**
     * get a short descriptions of this object, compatable with python __repr__.
     */
    virtual std::string toRepr() const;

    /**
     * get the name of this integrator
     */
    virtual std::string getName() const;

    /**
     * list of keys that this integrator supports.
     */
    static const Dictionary* getIntegratorOptions();

private:

    static const int mDefaultMaxNumSteps;

    N_Vector mStateVector;

    /**
     * the CVODE object.
     */
    void* mCVODE_Memory;

    static const int mDefaultMaxAdamsOrder;
    static const int mDefaultMaxBDFOrder;

    /**
     * the time that the last event occured.
     */
    double lastEventTime;

    /**
     * the shared model object, owned by SBMLSolver.
     */
    ExecutableModel* mModel;

    bool variableStepPendingEvent;

    bool variableStepTimeEndEvent;

    /**
     * If an event occurs at the exact moment a time step completes,
     * need to store the post event state as the pre-event state
     * is copied into the model to produce an extra time point just
     * before the event.
     */
    double *variableStepPostEventState;

    /**
     * an array of event status. Used by integrate to read the current model
     * event status into.
     */
    std::vector<unsigned char> eventStatus;

    /**
     * the listener
     */
    IntegratorListenerPtr listener;

    /**
     * apply any events before time = 0.
     *
     * Note side effect of setting the current cvode state into the model.
     */
    void testRootsAtInitialTime();

    /**
     * does the model have any state variables.
     */
    bool haveVariables();


    /**
     * copy the values from the cvode state vector into the executable model.
     */
    void assignResultsToModel();

    /**
     * Update the abl tolerance vector using the abs tol in the options.
     *
     * sets these values in the cvode object.
     */
    void setCVODETolerances();

    void applyPendingEvents(double timeEnd);

    void applyEvents(double timeEnd,
            std::vector<unsigned char> &previousEventStatus);


    double applyVariableStepPendingEvents();

    /**
     * re-initialize cvode with a new set of initial conditions
     */
    void reInit (double t0);

    /**
     * Set up the cvode state vector size and various other cvode
     * init tasks. Specific to the model.
     *
     * The existing cvode vars: mStateVector, mAbstolArray, mCVODE_Memory
     * must be 0 before calling.
     */
    void createCVode();

    /**
     * free and nullify the cvode objects.
     */
    void freeCVode();

    int mMaxAdamsOrder;
    int mMaxBDFOrder;

    /**
     * models may have no state vector variables, but in this case,
     * we still need a cvode state vector of len 1 for the integrator to
     * work.
     *
     * true if model has state vector, false otherwise
     */
    bool stateVectorVariables;

    /**
     * pointer to an options struct, this is typically
     * owned by the SBMLSolver object.
     */
    SimulateOptions options;

    /**
     * cvode dydt callback
     */
    friend int cvodeDyDtFcn(double t, N_Vector cv_y, N_Vector cv_ydot, void *f_data);

    /**
     * cvode event root finding callback.
     */
    friend int cvodeRootFcn (double t, N_Vector y, double *gout, void *g_data);
};
}

#endif
