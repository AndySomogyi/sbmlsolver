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

class Event;
class ExecutableModel;
class RoadRunner;

/**
 * @internal
 * The integrator implemented by CVODE.
 */
class CvodeInterface : public Integrator, public Configurable
{
public:
    double                      mMaxStep;
    int                         mRootCount;
    int                         mCount;

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


    virtual unsigned setTolerances(double relative, double absolute);
    virtual unsigned getTolerances(double *relative, double *absolute);

    void                        assignResultsToModel();

    void                        testRootsAtInitialTime();
    bool                        haveVariables();

    double                      oneStep(const double& timeStart, const double& hstep);
    void                        assignNewVector(ExecutableModel *model);
    void                        assignNewVector(ExecutableModel *oModel, bool bAssignNewTolerances);

    // Restart the simulation using a different initial condition
    void                        reStart(double timeStart, ExecutableModel* model);

private:

    const double                mDefaultReltol;
    const double                mDefaultAbsTol;
    static const int            mDefaultMaxNumSteps;

    string                      mTempPathstring;
    int                         mErrorFileCounter;

    int                         mStateVectorSize;
    N_Vector                    mStateVector;

    N_Vector                    mAbstolArray;

    void*                       mCVODE_Memory;

    static const int            mDefaultMaxAdamsOrder;
    static const int            mDefaultMaxBDFOrder;
    double                      mLastTimeValue;
    double                      mLastEvent;
    ExecutableModel*            mModel;
    int                         mOneStepCount;
    bool                        mFollowEvents;

    void                        handleCVODEError(const int& errCode);
    void                        assignPendingEvents(const double& timeEnd, const double& tout);


    void                        handleRootsForTime(double timeEnd,
                                    std::vector<unsigned char> &previousEventStatus);

    int                         rootInit (const int& numRoots);
    int                         reInit (const double& t0);
    int                         allocateCvodeMem ();
    void                        initializeCVODEInterface(ExecutableModel *oModel);
    void                        setAbsTolerance(int index, double dValue);

    int                         mMaxAdamsOrder;
    int                         mMaxBDFOrder;
    double                      mInitStep;
    double                      mMinStep;

    int                         mMaxNumSteps;


    double                      mRelTol;
    double                      mAbsTol;


    /**
     * pointer to an options struct, this is typically
     * owned by the RoadRunner object.
     */
    const SimulateOptions* options;

    friend void ModelFcn(int n, double time, double* y, double* ydot, void* userData);
    friend void EventFcn(double time, double* y, double* gdot, void* userData);

};
}

#endif
