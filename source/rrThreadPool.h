#ifndef rrThreadPoolH
#define rrThreadPoolH
#include <list>
#include <vector>
#include "Poco/Thread.h"
#include "Poco/Mutex.h"
#include "Poco/ScopedLock.h"
#include "rrRoadRunnerThread.h"
#include "rrLoadSBMLThread.h"
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


class LoadSBMLThreadPool : public ThreadPool
{
    public:
	    			LoadSBMLThreadPool(const int& nrThreads, const string& model, vector<RoadRunner*> &rrInstance) : ThreadPool()
                    {
                    	//create nrThreads that can load SBML models
                        for(int i = 0; i < nrThreads; i++)
                        {
                            mThreads.push_back(new LoadSBML(model));
                        }

                        //The following will add jobs and get them done too..
                        for(int i = 0; i < rrInstance.size(); i++)
                        {
                        	addJob(rrInstance[i]);
                        }

                    }
};

class SimulateSBMLThreadPool : public ThreadPool
{
    public:
	    			SimulateSBMLThreadPool(const int& nrThreads, vector<RoadRunner*> &rrInstance) : ThreadPool()
                    {
                    	//create nrThreads that can load SBML models
                        for(int i = 0; i < nrThreads; i++)
                        {
                            mThreads.push_back(new Simulate());
                        }

                        //The following will add jobs and get them done too..
                        for(int i = 0; i < rrInstance.size(); i++)
                        {
                        	addJob(rrInstance[i]);
                        }
                    }
};




}
#endif
