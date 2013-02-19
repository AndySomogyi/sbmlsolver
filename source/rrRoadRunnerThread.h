#ifndef rrRoadRunnerThreadH
#define rrRoadRunnerThreadH
#include <list>
#include "Poco/Thread.h"
#include "Poco/Mutex.h"
#include "Poco/Condition.h"
#include "Poco/Runnable.h"
#include "rrObject.h"
//---------------------------------------------------------------------------
namespace rr
{
using std::list;

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
		void				        start();
    	virtual void                run();
		virtual void                worker() = 0; 		//Have to derive from worker!
        virtual void				signalAll() = 0;
		virtual void 	            addJob(RoadRunner* instance) = 0;
        virtual void				signalExit() = 0;
        virtual unsigned int  		getNrOfJobsInQueue() = 0;
        void				        exit();

        void				        join();
        bool				        isActive();


};

}
#endif
