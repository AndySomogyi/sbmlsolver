#ifndef lm_threadH
#define lm_threadH
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "rrRoadRunner.h"
#include "rrMinimizationResult.h"
//---------------------------------------------------------------------------

typedef void (__stdcall *ThreadCB)(void*);

class LM;
class rr::RoadRunnerData;

class rr::RoadRunner;
extern rr::RoadRunner *rri;

class LMFitThread : public Poco::Runnable
{
	protected:
   		Poco::Thread 		       	mThread;

		//Callbacks
        ThreadCB  			       	threadEnterCB;
        ThreadCB  			       	threadExitCB;
        void*				       	mUserData;   	//Used for plugin callbacks..

        LM&     					mTheHost;
//        rr::RoadRunnerData	       *mInputData;		//This is Experimental input data
        rr::MinimizationResult     *mResult;

	public:
							       	LMFitThread(LM& host);
      	void				       	assignCallBacks(ThreadCB fn1, ThreadCB fn2, void* userData);
        void				       	start(rr::RoadRunnerData* inputData);
        void				   		run();
};


#endif
