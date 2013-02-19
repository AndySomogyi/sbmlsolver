#pragma hdrstop
#include "rrSimulateThread.h"
#include "rrLogger.h"
#include "rrRoadRunner.h"
//---------------------------------------------------------------------------

namespace rr
{


list<RoadRunner*>   SimulateThread::mJobs;
Mutex 				SimulateThread::mJobsMutex;
Condition			SimulateThread::mJobsCondition;

SimulateThread::SimulateThread()
:
RoadRunnerThread()
{
    start();
}

void SimulateThread::addJob(RoadRunner* rr)
{
	//getMutex
    Mutex::ScopedLock lock(mJobsMutex);
    mJobs.push_back(rr);
	mJobsCondition.signal();	//Tell the thread its time to go to work
}

void SimulateThread::worker()
{
    RoadRunner *rri = NULL;
    while(!mIsTimeToDie)
    {
        {	//Scope for the mutex lock...
            Mutex::ScopedLock lock(mJobsMutex);
            if(mJobs.size() == 0)
            {
                Log(lDebug5)<<"Waiting for jobs in SimulateThread worker";
                mJobsCondition.wait(mJobsMutex);
            }

            if(mJobs.size() == 0 || mIsTimeToDie)
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
            Log(lInfo)<<"Simulating RR instance: "<<rri->getInstanceID();
            if(!rri->simulate2())
            {
                Log(lError)<<"Failed simulating instance: "<<rri->getInstanceID();
            }
        }

    }

    Log(lInfo)<<"Exiting simulate thread: "<<mThread.id();
}

void SimulateThread::signalExit()
{
	mJobsCondition.signal();
}

unsigned int SimulateThread::getNrOfJobsInQueue()
{
    Mutex::ScopedLock lock(mJobsMutex);
    return mJobs.size();
}

void SimulateThread::signalAll()
{
	mJobsCondition.broadcast();
}



}