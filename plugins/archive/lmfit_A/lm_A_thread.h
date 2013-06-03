#ifndef lm_A_threadH
#define lm_A_threadH
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "rrRoadRunner.h"
#include "rrMinimizationData.h"
//---------------------------------------------------------------------------

typedef void (__stdcall *ThreadCB)(void*);

class LM_A;
class rr::RoadRunner;
class rr::RoadRunnerData;
using rr::MinimizationData;

class LM_A_Thread : public Poco::Runnable
{
	protected:
   		Poco::Thread 		       	mThread;

		//Callbacks
        ThreadCB  			       	threadEnterCB;
        ThreadCB  			       	threadExitCB;
        void*				       	mUserData;   	//Used for plugin callbacks..

        LM_A&     					mTheHost;
        MinimizationData&       	mMinData;
		bool					    setupRoadRunner();

	public:
							       	LM_A_Thread(LM_A& host);
      	void				       	assignCallBacks(ThreadCB fn1, ThreadCB fn2, void* userData);
        void				       	start();
        void				   		run();
        bool						isRuning();
};


#endif
