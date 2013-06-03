#ifndef rrLoadModelThreadH
#define rrLoadModelThreadH
#include <list>
#include "rrExporter.h"
#include "rrRoadRunnerThread.h"
#include "Poco/Condition.h"

//---------------------------------------------------------------------------
using std::list;

namespace rr
{

class RR_DECLSPEC LoadModelThread : public RoadRunnerThread
{
	protected:
       	static list<RoadRunner*>    mJobs;
		static Poco::Mutex	 		mJobsMutex;
        static Poco::Condition		mJobsCondition;
		static Poco::Mutex	 		mNrOfWorkersMutex;
        static int					mNrOfWorkers;		//incremented when entering worker function and decremented when exiting

	protected:
		string						mModelFileName;
		string						mSBML;				//If not loading from file
        bool						mRecompileOnLoad;

        void						signalAll();
        void						signalExit();

	public:
    					            LoadModelThread(const string& modelFile = gEmptyString, bool recompileOnLoad = true, RoadRunner* rri = NULL, bool autoStart = false);
    					           ~LoadModelThread();
		void						setSBML(const string& sbml);
    	void 			            worker();
		void 			            addJob(RoadRunner* rr);
		unsigned int  				getNrOfJobsInQueue();
        bool	  					isAnyWorking();
};

}
#endif
