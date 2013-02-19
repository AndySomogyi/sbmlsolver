#ifndef rrLoadSBMLThreadH
#define rrLoadSBMLThreadH
#include "Poco/Thread.h"
#include "Poco/Mutex.h"
#include "Poco/ScopedLock.h"
#include "rrExporter.h"
#include "rrRoadRunnerThread.h"
//---------------------------------------------------------------------------

namespace rr
{

class RR_DECLSPEC LoadSBML : public RoadRunnerThread
{
	protected:
		string			mModelFileName;
    	static list<RoadRunner*>    mJobs;
		static Poco::Mutex	 		mJobsMutex;
        static Poco::Condition		mJobsCondition;
        void						signalAll();

	public:
    					            LoadSBML(const string& modelFile);
    	void 			            worker();
		void 			            addJob(RoadRunner* rr);
        void						signalExit();
		unsigned int  				getNrOfJobsInQueue();

};

class Simulate : public RoadRunnerThread
{

	protected:
        static list<RoadRunner*>    mJobs;
		static Poco::Mutex	 		mJobsMutex;
    	static Poco::Condition		mJobsCondition;
        void						signalAll();
	public:
    								Simulate();
    	void 						worker();
		void 			            addJob(RoadRunner* rr);
        void						signalExit();
		unsigned int  				getNrOfJobsInQueue();
};


}
#endif
