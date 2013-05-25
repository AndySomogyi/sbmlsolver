#ifndef rrThreadPoolH
#define rrThreadPoolH
#include <list>
#include <vector>
#include "rrRoadRunnerThread.h"
//---------------------------------------------------------------------------


namespace rr
{
using namespace std;

class RR_DECLSPEC ThreadPool
{
	protected:
		list<RoadRunnerThread*>		mThreads;

    public:
								   	ThreadPool();
		virtual					   ~ThreadPool();
		void						addJob(RoadRunner* rri);
		bool						isJobQueueEmpty();
		int							getNumberOfRemainingJobs();
		bool						isWorking();
		void						start();
		void						exitAll();
		void						waitForStart();
		void						waitForFinish();
};

}
#endif
