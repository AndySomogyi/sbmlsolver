#ifndef lm_threadH
#define lm_threadH
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "rrRoadRunner.h"
#include "rrMinimizationData.h"
#include "lmUtils.h"
//---------------------------------------------------------------------------

namespace lm
{

class LM;

using rr::RoadRunnerData;
using rrp::MinimizationData;

class LMFitThread : public Poco::Runnable
{
    protected:
        lmDataStructure             mLMData;        //LevenbergMarq.. data
        Poco::Thread                mThread;

        LM&                         mTheHost;
        MinimizationData&           mMinData;
        rr::RoadRunner              *mRRI;
        bool                        setupRoadRunner();
        bool                        setup();
        RoadRunnerData              createModelData();
        RoadRunnerData              createResidualsData();

    public:
                                    LMFitThread(LM& host);
        void                        start(bool runInThread = true);
        void                        run();
        bool                        isRunning();
};

}
#endif
