#ifndef full_space_fit_threadH
#define full_space_fit_threadH
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "rrMinimizationResult.h"
//---------------------------------------------------------------------------

namespace fullSpaceFit
{
typedef void    (__stdcall *ThreadCB)(void*);

class FullSpaceMinimization;
class rr::RoadRunnerData;



class FullSpaceFitThread : public Poco::Runnable
{
	protected:
   		Poco::Thread 		       	mThread;

		//Callbacks
        ThreadCB  			       	threadEnterCB;
        ThreadCB  			       	threadExitCB;
        void*				       	mUserData;   	//Used for plugin callbacks..

        FullSpaceMinimization&     	mTheHost;
        rr::RoadRunnerData	       *mInputData;		//This is Experimental input data
        rr::MinimizationResult&    	mResult;

	public:
							       	FullSpaceFitThread(FullSpaceMinimization& host);
      	void				       	assignCallBacks(ThreadCB fn1, ThreadCB fn2, void* userData);
        void				       	start(rr::RoadRunnerData* inputData);
        void				   		run();
};

}
#endif
