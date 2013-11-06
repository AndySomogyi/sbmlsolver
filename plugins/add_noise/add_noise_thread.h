#ifndef add_noise_threadH
#define add_noise_threadH
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "rrPluginsAPIExporter.h"
//---------------------------------------------------------------------------

typedef void (callback_cc *ThreadCB)(void*);

class AddNoiseThread : public Poco::Runnable
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
                                AddNoiseThread();
        void                    assignCallBacks(ThreadCB fn1, ThreadCB fn2, void* userData);
        bool                    start(void* inputData, double sigma, bool runInThread = true);
        void                    run();
        bool                    isRunning();
};

#endif
