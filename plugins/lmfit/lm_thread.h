#ifndef lm_threadH
#define lm_threadH
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "rrRoadRunner.h"
#include "rrMinimizationData.h"
//---------------------------------------------------------------------------

typedef void (__stdcall *ThreadCB)(void*);

class LM;
class rr::RoadRunner;
class rr::RoadRunnerData;

using rr::MinimizationData;

//extern rr::RoadRunner *rri;

class LMFitThread : public Poco::Runnable
{
	protected:
   		Poco::Thread 		       	mThread;

		//Callbacks
        ThreadCB  			       	threadEnterCB;
        ThreadCB  			       	threadExitCB;
        void*				       	mUserData;   	//Used for plugin callbacks..

        LM&     					mTheHost;
        MinimizationData&       	mMinData;

	public:
							       	LMFitThread(LM& host);
      	void				       	assignCallBacks(ThreadCB fn1, ThreadCB fn2, void* userData);
        void				       	start();
        void				   		run();
        bool						isRuning();
};


#endif
