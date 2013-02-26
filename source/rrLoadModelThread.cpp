#pragma hdrstop
#include "Poco/Mutex.h"
#include "Poco/ScopedLock.h"
#include "rrLogger.h"
#include "rrLoadModelThread.h"
#include "rrRoadRunner.h"
//---------------------------------------------------------------------------

namespace rr
{

using namespace Poco;

list<RoadRunner*>   LoadModelThread::mJobs;
Poco::Mutex 		LoadModelThread::mJobsMutex;
Poco::Condition		LoadModelThread::mJobsCondition;


LoadModelThread::LoadModelThread(const string& modelFile)
:
mModelFileName(modelFile)
{
    start();
}

void LoadModelThread::addJob(RoadRunner* rr)
{
	//getMutex
    Mutex::ScopedLock lock(mJobsMutex);
    mJobs.push_back(rr);
	mJobsCondition.signal();	//Tell the thread its time to go to work
}

unsigned int LoadModelThread::getNrOfJobsInQueue()
{
    Mutex::ScopedLock lock(mJobsMutex);
    return mJobs.size();
}

void LoadModelThread::signalExit()
{
	mJobsCondition.signal();
}

void LoadModelThread::signalAll()
{
	mJobsCondition.broadcast();
}

void LoadModelThread::worker()
{
    RoadRunner *rri = NULL;
    while(!mIsTimeToDie)
    {
        {	//Scope for scoped lock
           	Mutex::ScopedLock lock(mJobsMutex);
            if(mJobs.size() == 0)
            {
                Log(lDebug5)<<"Waiting for jobs in loadSBML worker";
                mJobsCondition.wait(mJobsMutex);
            }

            if(mIsTimeToDie)
            {
                break;	//ends the life of the thread..
            }
            else
            {
                //Get a job
                rri = mJobs.front();
                mJobs.pop_front();
            }
        }		//Causes the scoped lock to unlock

        //Do the job
        if(rri)
        {
            Log(lInfo)<<"Loading model into instance: "<<rri->getInstanceID();
            rri->loadSBMLFromFile(mModelFileName);
        }
    }

    Log(lInfo)<<"Exiting thread: "<<mThread.id();
}

}

