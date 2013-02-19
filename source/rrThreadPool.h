#ifndef rrThreadPoolH
#define rrThreadPoolH
#include <list>
#include "Poco/Thread.h"
#include "Poco/Mutex.h"
#include "Poco/ScopedLock.h"
#include "rrRoadRunnerThread.h"
//---------------------------------------------------------------------------
using namespace std;
using namespace rr;
using Poco::Mutex::ScopedLock;

namespace rr
{

class ThreadPool
{
	protected:
		list<RoadRunnerThread*>		mThreads;

    public:
								   	ThreadPool();
		void						addJob(RoadRunner* rri);
		bool						isJobQueueEmpty();
		bool						isActive();
		void						exitAll();
		void						waitForAll();

};
}
#endif
