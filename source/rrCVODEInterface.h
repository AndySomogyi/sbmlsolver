#ifndef rrCvodeInterfaceH
#define rrCvodeInterfaceH
#include <string>
#include "rrCapability.h"
#include "rrParameter.h"
#include "Integrator.h"

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

class CvodeInterface : public Integrator
{
public:
    double                      mMaxStep;
    int                         mRootCount;
    int                         mCount;

    CvodeInterface(ExecutableModel* oModel, double relTol = 1.e-12,
            double abTol = 1.e-12);

    virtual ~CvodeInterface();


    Capability&                 getCapability();    //Only one capability


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
                                    vector<unsigned char> &previousEventStatus);

    int                         rootInit (const int& numRoots);//, TRootCallBack callBack, void *gdata);
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

    Capability                  mCVODECapability;

    /**
     * parameters we add to the capabities list. Just allocate them as ivars
     * so we don't have to delete them later.
     *
     * These have to be declared last in this file as their initial values
     * are dependent on ivars listed earlier.
     */
    Parameter<int> paramBDFOrder;
    Parameter<int> paramAdamsOrder;
    Parameter<double> paramRTol;
    Parameter<double> paramATol;
    Parameter<int> paramMaxSteps;
    Parameter<double> paramInitSteps;
    Parameter<double> paramMinStep;
    Parameter<double> paramMaxStep;

    friend void ModelFcn(int n, double time, double* y, double* ydot, void* userData);
    friend void EventFcn(double time, double* y, double* gdot, void* userData);

};
}

#endif
