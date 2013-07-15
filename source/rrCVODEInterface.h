#ifndef rrCvodeInterfaceH
#define rrCvodeInterfaceH
#include <string>
#include "rrObject.h"
#include "rrPendingAssignment.h"
#include "rrCapability.h"
#include "rrParameter.h"
#include "cvode/cvode.h"

namespace rr
{

using std::string;

class Event;
class ExecutableModel;
class RoadRunner;

class RR_DECLSPEC CvodeInterface
{
    private:
        const double                mDefaultReltol;
        const double                mDefaultAbsTol;
        const int                   mDefaultMaxNumSteps;

        string                        mTempPathstring;
        int                          mErrorFileCounter;
        int                         mNumIndependentVariables;
        N_Vector                    mAmounts;
        N_Vector                    mAbstolArray;
        string                      mLogFile;
        void*                       mCVODE_Memory;
        int                         mNumAdditionalRules;
        vector<double>              mAssignmentTimes;
        int                         mDefaultMaxAdamsOrder;
        int                         mDefaultMaxBDFOrder;
        double                      mLastTimeValue;
        double                      mLastEvent;
        ExecutableModel*                    mTheModel;
        int                         mOneStepCount;
        bool                        mFollowEvents;
        RoadRunner                   *mRR;

        void                        handleCVODEError(const int& errCode);
        void                        assignPendingEvents(const double& timeEnd, const double& tout);
        vector<int>                 retestEvents(const double& timeEnd, vector<int>& handledEvents);
        vector<int>                 retestEvents(const double& timeEnd, const vector<int>& handledEvents, vector<int>& removeEvents);
        vector<int>                 retestEvents(const double& timeEnd, vector<int>& handledEvents, const bool& assignOldState);
        vector<int>                 retestEvents(const double& timeEnd, const vector<int>& handledEvents, const bool& assignOldState, vector<int>& removeEvents);
        void                        handleRootsFound(double &timeEnd, const double& tout);
        void                        removePendingAssignmentForIndex(const int& eventIndex);
        void                        sortEventsByPriority(vector<int>& firedEvents);
        void                        sortEventsByPriority(vector<Event>& firedEvents);
        void                        handleRootsForTime(const double& timeEnd, vector<int>& rootsFound);
         int                         rootInit (const int& numRoots);//, TRootCallBack callBack, void *gdata);
        int                         reInit (const double& t0);
        int                          allocateCvodeMem ();
        void                        initializeCVODEInterface(ExecutableModel *oModel);
        void                        setAbsTolerance(int index, double dValue);


    public:    //Hide these later on...
        int                         mMaxAdamsOrder;
        int                         mMaxBDFOrder;
        double                      mInitStep;
        double                      mMinStep;
        double                      mMaxStep;
        int                         mMaxNumSteps;
        double                      mRelTol;
        double                      mAbsTol;
        vector<PendingAssignment>   mAssignments;
        int                          mRootCount;
        int                         mCount;
        Capability                    mCVODECapability;

    public:
                                    CvodeInterface(RoadRunner* rr, ExecutableModel* oModel, const double& abTol = 1.e-12, const double& relTol = 1.e-12);
                                   ~CvodeInterface();

        Capability&                    getCapability();    //Only one capability

        void                         setTolerances(const double& aTol, const double& rTol);
        void                        assignResultsToModel();
        ExecutableModel*                    getModel();
        void                        testRootsAtInitialTime();
        bool                        haveVariables();

        double                      oneStep(const double& timeStart, const double& hstep);
        vector<double>              buildEvalArgument();
        void                        assignNewVector(ExecutableModel *model);
        void                        assignNewVector(ExecutableModel *oModel, bool bAssignNewTolerances);

                                    // Restart the simulation using a different initial condition
        void                        reStart(double timeStart, ExecutableModel* model);

    private:
        /**
         * parameters we add to the capabities list. Just allocate them as ivars
         * so we don't have to delete them later.
         */
        Parameter<int> paramBDFOrder;
        Parameter<int> paramAdamsOrder;
        Parameter<double> paramRTol;
        Parameter<double> paramATol;
        Parameter<int> paramMaxSteps;
        Parameter<double> paramInitSteps;
        Parameter<double> paramMinStep;
        Parameter<double> paramMaxStep;
};
}

#endif
