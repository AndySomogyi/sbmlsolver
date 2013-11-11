#ifndef add_noise_workerH
#define add_noise_workerH
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "rrPluginsAPIExporter.h"
//---------------------------------------------------------------------------

typedef void (callback_cc *ThreadCB)(void*);

class AddNoiseWorker : public Poco::Runnable
{
    protected:
        Poco::Thread            mThread;

        //Callbacks
        ThreadCB                threadEnterCB;
        ThreadCB                threadProgressCB;
        ThreadCB                threadExitCB;

        void*                   mUserData;
        void*                   mInputData;
        double                  mSigma;

    public:
                                AddNoiseWorker();
        void                    assignCallBacks(ThreadCB fn1, ThreadCB fn2, ThreadCB fn3, void* userData = NULL);
        bool                    start(void* inputData, double sigma, bool runInThread = true);
        void                    run();
        bool                    isRunning();
};

#endif
