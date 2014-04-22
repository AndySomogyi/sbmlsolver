#ifndef rrCvodeInterfaceH
#define rrCvodeInterfaceH

#include "Integrator.h"
#include "rrRoadRunnerOptions.h"
#include "Configurable.h"

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
class RoadRunner;

/**
 * @internal
 * The integrator implemented by CVODE.
 */
class CvodeInterface : public Integrator, public Configurable
{
public:
    CvodeInterface(ExecutableModel* oModel, const SimulateOptions* options);

    virtual ~CvodeInterface();

    /**
     * creates a new xml element that represent the current state of this
     * Configurable object and all if its child objects.
     */
    virtual _xmlNode *createConfigNode();

    /**
     * Given an xml element, the Configurable object should pick its needed
     * values that are stored in the element and use them to set its
     * internal configuration state.
     */
    virtual void loadConfig(const _xmlDoc* doc);

    double integrate(double t0, double tf);

    /**
     * copies the state vector out of the model and into cvode vector,
     * re-initializes cvode.
     */
    void reStart(double timeStart);

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

    // TODO: these need to made private.
    void testRootsAtInitialTime();
    bool haveVariables();

private:

    static const int mDefaultMaxNumSteps;

    N_Vector mStateVector;

    N_Vector mAbstolArray;

    void* mCVODE_Memory;

    static const int mDefaultMaxAdamsOrder;
    static const int mDefaultMaxBDFOrder;
    double mLastTimeValue;
    double mLastEvent;

    /**
     * the shared model object, owned by RoadRunner.
     */
    ExecutableModel* mModel;

    int mOneStepCount;

    bool mFollowEvents;

    /**
     * the listener
     */
    IntegratorListenerPtr listener;

    /**
     * copy the values from the cvode state vector into the executable model.
     */
    void assignResultsToModel();

    /**
     * Update the abl tolerance vector using the abs tol in the options.
     *
     * Play some games if the value of the state vector is small, adjust the
     * tolerance accordingly.
     *
     * Side effect is model state is copied into cvode state vector.
     */
    void updateAbsTolVector();

    void assignPendingEvents(double timeEnd, double tout);

    void handleRootsForTime(double timeEnd,
            std::vector<unsigned char> &previousEventStatus);

    /**
     * re-initialize cvode with a new set of initial conditions
     */
    int reInit (double t0);

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

    void setAbsTolerance(int index, double dValue);

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
     * owned by the RoadRunner object.
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
