#ifndef rrRoadRunnerThreadH
#define rrRoadRunnerThreadH
#include <list>
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "rrObject.h"
//---------------------------------------------------------------------------
using std::list;

namespace rr
{
class RoadRunner;

class RR_DECLSPEC RoadRunnerThread : public Poco::Runnable, public rrObject
{
	protected:
	    Poco::Thread 				mThread;
        bool						mIsTimeToDie;

    public:
	    					        RoadRunnerThread();

		void				        setName(const string& name);
		string 				        getName();
        void				        join();
        bool				        isActive();
		void				        start();
    	virtual void                run();
        void				        exit();	//Tell the thread to die

		//Pure virtuals
		virtual void                worker() = 0;
		virtual void 	            addJob(RoadRunner* instance) = 0;
        virtual unsigned int  		getNrOfJobsInQueue() = 0;


        //Todo: Gotta refine the exiting of threads later...
        virtual void				signalExit() = 0;
        virtual void				signalAll() = 0;

};

}
#endif
