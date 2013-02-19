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
    	static list<RoadRunner*>    mJobs;
		static Poco::Mutex	 		mJobsMutex;
        static Poco::Condition		mJobsCondition;
        bool						mIsTimeToDie;


    public:
	    					        RoadRunnerThread();
		void 				        addJob(RoadRunner* instance);
		void				        setName(const string& name);
		string 				        getName();
		void				        start();
    	virtual void                run();
		virtual void                worker() = 0; 		//Have to derive from worker!
        void				        exit();
        void				        join();
        bool				        isActive();
        unsigned int  		        getNrOfJobsInQueue();
        void						signalAll();
};

}
#endif
