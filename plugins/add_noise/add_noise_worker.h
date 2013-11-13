#ifndef add_noise_workerH
#define add_noise_workerH
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "rrPluginsAPIExporter.h"
//---------------------------------------------------------------------------

typedef void (callback_cc *ThreadCB)(void*);

namespace addNoise
{

class AddNoise;

class AddNoiseWorker : public Poco::Runnable
{
    protected:
        Poco::Thread            mThread;

//        //Callbacks
//        ThreadCB                workerEnterCB;
//        ThreadCB                workerProgressCB;
//        ThreadCB                workerExitCB;
//
//        void*                   mUserDataEnterCB;
//        void*                   mUserDataProgressCB;
//        void*                   mUserDataExitCB;

        void*                   mInputData;
        double                  mSigma;
        AddNoise&               mTheHost;
    public:
                                AddNoiseWorker(AddNoise& mTheHost);
        bool                    start(void* inputData, double sigma, bool runInThread = true);
        void                    run();
        bool                    isRunning();
};

}
#endif
