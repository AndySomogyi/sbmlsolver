#ifndef full_space_fit_threadH
#define full_space_fit_threadH
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
//---------------------------------------------------------------------------

namespace fullSpaceFit
{
typedef void    (__stdcall *ThreadCB)(void*);
class FullSpaceMinimization;
class rr::SimulationData;

class FullSpaceFitThread : public Poco::Runnable
{
	protected:
   		Poco::Thread 			mThread;

		//Callbacks
        ThreadCB  				threadEnterCB;
        ThreadCB  				threadExitCB;
        void*					mUserData;   	//Used for plugin callbacks..

        rr::SimulationData	   *mInputData;		//This is Experimental input data
        FullSpaceMinimization&	mTheHost;

	public:
								FullSpaceFitThread(FullSpaceMinimization& host);
      	void					assignCallBacks(ThreadCB fn1, ThreadCB fn2, void* userData);
        void					start(rr::SimulationData* inputData);
        void					run();
};

}
#endif
