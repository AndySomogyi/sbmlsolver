#ifndef lm_threadH
#define lm_threadH
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "rrRoadRunner.h"
#include "rrMinimizationData.h"
#include "lmUtils.h"
//---------------------------------------------------------------------------

typedef void (rrpCallConv *ThreadCB)(void*);

class LM;

using rr::RoadRunnerData;
using rrp::MinimizationData;


class LMFitThread : public Poco::Runnable
{
    protected:
        lmDataStructure             mLMData;        //LevenbergMarq.. data
        Poco::Thread                mThread;

        //Callbacks
        ThreadCB                    threadEnterCB;
        ThreadCB                    threadExitCB;
        void*                       mUserData;       //Used for plugin callbacks..

        LM&                         mTheHost;
        MinimizationData&           mMinData;
        rr::RoadRunner              *mRRI;
        bool                        setupRoadRunner();
        bool                        setup();
        RoadRunnerData              createModelData();
        RoadRunnerData              createResidualsData();

    public:
                                    LMFitThread(LM& host);
        void                        assignCallBacks(ThreadCB fn1, ThreadCB fn2, void* userData);
        void                        start();
        void                        run();
        bool                        isRunning();
};

#endif
