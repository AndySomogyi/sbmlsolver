#ifndef rrRoadRunnerThreadH
#define rrRoadRunnerThreadH
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "rrObject.h"
//---------------------------------------------------------------------------
namespace rr
{
class RoadRunner;


class RR_DECLSPEC RoadRunnerThread : public Poco::Runnable, public rrObject
{
	protected:
	    Poco::Thread 		mThread;
    	RoadRunner*			mRR;
        string				mThreadName;

    public:
	    					RoadRunnerThread(RoadRunner* rr);
		void				setThreadName(const string& name);
		void				start();
    	virtual void        run();
		virtual void        worker() = 0;
        void				join();
        bool				isRunning();
        RoadRunner*			getRRInstance();


};

}
#endif
