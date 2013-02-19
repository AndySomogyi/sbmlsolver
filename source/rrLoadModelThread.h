#ifndef rrLoadModelThreadH
#define rrLoadModelThreadH
#include <list>
#include "rrExporter.h"
#include "rrRoadRunnerThread.h"
//---------------------------------------------------------------------------
using std::list;

namespace rr
{

class RR_DECLSPEC LoadModelThread : public RoadRunnerThread
{
	protected:
		string						mModelFileName;
    	static list<RoadRunner*>    mJobs;
		static Poco::Mutex	 		mJobsMutex;
        static Poco::Condition		mJobsCondition;
        void						signalAll();
        void						signalExit();

	public:
    					            LoadModelThread(const string& modelFile);
    	void 			            worker();
		void 			            addJob(RoadRunner* rr);
		unsigned int  				getNrOfJobsInQueue();

};

}
#endif
