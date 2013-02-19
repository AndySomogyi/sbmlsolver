#ifndef rrLoadSBMLThreadH
#define rrLoadSBMLThreadH
#include "Poco/Thread.h"
#include "Poco/Mutex.h"
#include "Poco/Condition.h"
#include "Poco/ScopedLock.h"
#include "rrExporter.h"
#include "rrRoadRunnerThread.h"
//---------------------------------------------------------------------------

namespace rr
{

class RR_DECLSPEC LoadModel : public RoadRunnerThread
{
	protected:
		string						mModelFileName;
    	static list<RoadRunner*>    mJobs;
		static Poco::Mutex	 		mJobsMutex;
        static Poco::Condition		mJobsCondition;
        void						signalAll();
        void						signalExit();

	public:
    					            LoadModel(const string& modelFile);
    	void 			            worker();
		void 			            addJob(RoadRunner* rr);
		unsigned int  				getNrOfJobsInQueue();

};

class Simulate : public RoadRunnerThread
{

	protected:
        static list<RoadRunner*>    mJobs;
		static Poco::Mutex	 		mJobsMutex;
    	static Poco::Condition		mJobsCondition;
        void						signalAll();
        void						signalExit();

	public:
    								Simulate();
    	void 						worker();
		void 			            addJob(RoadRunner* rr);
		unsigned int  				getNrOfJobsInQueue();
};

}
#endif
