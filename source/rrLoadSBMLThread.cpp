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

list<RoadRunner*>   LoadModel::mJobs;
Poco::Mutex 		LoadModel::mJobsMutex;
Poco::Condition		LoadModel::mJobsCondition;

list<RoadRunner*>   Simulate::mJobs;
Poco::Mutex 		Simulate::mJobsMutex;
Poco::Condition		Simulate::mJobsCondition;

LoadModel::LoadModel(const string& modelFile)
:
mModelFileName(modelFile)
{
    start();
}

void LoadModel::addJob(RoadRunner* rr)
{
	//getMutex
    Mutex::ScopedLock lock(mJobsMutex);
    mJobs.push_back(rr);
	mJobsCondition.signal();	//Tell the thread its time to go to work
}

unsigned int LoadModel::getNrOfJobsInQueue()
{
    Mutex::ScopedLock lock(mJobsMutex);
    return mJobs.size();
}

void LoadModel::signalExit()
{
	mJobsCondition.signal();
}

void LoadModel::signalAll()
{
	mJobsCondition.broadcast();
}

void LoadModel::worker()
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

