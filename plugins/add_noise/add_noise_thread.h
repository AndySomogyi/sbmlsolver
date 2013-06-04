#ifndef add_noise_threadH
#define add_noise_threadH
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "rrExporter.h"
//---------------------------------------------------------------------------



typedef void (rrCallConv *ThreadCB)(void*);


class RR_DECLSPEC AddNoiseThread : public Poco::Runnable
{
    protected:
        Poco::Thread             mThread;

        //Callbacks
        ThreadCB                 threadEnterCB;
        ThreadCB                 threadExitCB;
        void*                    mUserData;

        void*                    mInputData;
        double                   mSigma;

    public:
                                 AddNoiseThread();
        void                     assignCallBacks(ThreadCB fn1, ThreadCB fn2, void* userData);
        void                     start(void* inputData, double sigma);
        void                     run();
};


#endif
