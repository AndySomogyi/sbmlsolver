#pragma hdrstop
#include "Poco/Mutex.h"
#include "Poco/ScopedLock.h"
#include "rrLogger.h"
#include "rrLoadSBMLThread.h"
#include "rrRoadRunner.h"
//---------------------------------------------------------------------------

namespace rr
{

using namespace Poco;

list<RoadRunner*>   LoadSBML::mJobs;
Poco::Mutex 		LoadSBML::mJobsMutex;
Poco::Condition		LoadSBML::mJobsCondition;

list<RoadRunner*>   Simulate::mJobs;
Poco::Mutex 		Simulate::mJobsMutex;
Poco::Condition		Simulate::mJobsCondition;

LoadSBML::LoadSBML(const string& modelFile)
:
mModelFileName(modelFile)
{
    start();
}

void LoadSBML::addJob(RoadRunner* rr)
{
	//getMutex
    Mutex::ScopedLock lock(mJobsMutex);
    mJobs.push_back(rr);
	mJobsCondition.signal();	//Tell the thread its time to go to work
}

unsigned int LoadSBML::getNrOfJobsInQueue()
{
    Mutex::ScopedLock lock(mJobsMutex);
    return mJobs.size();
}

void LoadSBML::signalExit()
{
	mJobsCondition.signal();
}

void LoadSBML::signalAll()
{
	mJobsCondition.broadcast();
}

void LoadSBML::worker()
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


Simulate::Simulate()
:
RoadRunnerThread()
{
    start();
}

void Simulate::addJob(RoadRunner* rr)
{
	//getMutex
    Mutex::ScopedLock lock(mJobsMutex);
    mJobs.push_back(rr);
	mJobsCondition.signal();	//Tell the thread its time to go to work
}

void Simulate::worker()
{
    RoadRunner *rri = NULL;
    while(!mIsTimeToDie)
    {
        {	//Scope for the mutex lock...
            Mutex::ScopedLock lock(mJobsMutex);
            if(mJobs.size() == 0)
            {
                Log(lDebug5)<<"Waiting for jobs in Simulate worker";
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

void Simulate::signalExit()
{
	mJobsCondition.signal();
}

unsigned int Simulate::getNrOfJobsInQueue()
{
    Mutex::ScopedLock lock(mJobsMutex);
    return mJobs.size();
}

void Simulate::signalAll()
{
	mJobsCondition.broadcast();
}



}

