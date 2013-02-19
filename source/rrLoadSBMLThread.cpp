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


LoadSBML::LoadSBML(const string& modelFile)
:
mModelFileName(modelFile)
{
    start();
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
                return;	//ends the life of the thread..
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
    Log(lDebug)<<"Exiting thread";
}

}

