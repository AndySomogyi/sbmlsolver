#pragma hdrstop
#include "rrLogger.h"
#include "rrThreadPool.h"
//---------------------------------------------------------------------------

namespace rr
{
ThreadPool::ThreadPool()
{}

void ThreadPool::addJob(RoadRunner* rri)
{
    //We add jobs to the threads (static) queue. Means that at least one thread has to exist
    if(mThreads.size() < 1)
    {
        return ;
    }
    else
    {
        if(mThreads.front() != NULL)
        {
            mThreads.front()->addJob(rri);
        }
    }
}

bool ThreadPool::isJobQueueEmpty()
{
    if(mThreads.front() != NULL)
    {
        bool val = mThreads.front()->getNrOfJobsInQueue() > 0 ? false : true;
        if(val == true)
        {
        	Log(lInfo)<<"Job queue is empty!";
        }
        return val;
    }
    return true;
}

int ThreadPool::getNumberOfRemainingJobs()
{
    if(mThreads.front() != NULL)
    {
        int val = mThreads.front()->getNrOfJobsInQueue();
        return val;
    }
    return -1;
}

bool ThreadPool::isActive()
{
    list<RoadRunnerThread*>::iterator	iter;
    for(iter = mThreads.begin(); iter != mThreads.end(); iter++)
    {
  	    	if((*iter) != NULL)
            {
				if((*iter)->isActive())
                {
                	return true;
                }
            }
	}
    return false;
}

void ThreadPool::exitAll()
{
    //Send each thread a time to die message
    list<RoadRunnerThread*>::iterator	iter;

    for(iter = mThreads.begin(); iter != mThreads.end(); iter++)
    {
        if((*iter) != NULL)
        {
	        (*iter)->exit();
	    }
    }

	iter = mThreads.begin();
    if((*iter) != NULL)
    {
    	(*iter)->signalAll();
    }
}

void ThreadPool::waitForAll()
{
    //Check that all jobs been done
    //This should be checked in a thread, and using a condition Variable
    while(isJobQueueEmpty() == false)
    {
        Poco::Thread::sleep(50);
    };

	Poco::Thread::sleep(50);	//We need a way to know if a thread is still doing processing...

    //Dispose threads..
    exitAll();

    //This could be checked in a thread, and using a condition Variable
    while(isActive() == true)
    {
	    exitAll(); //Ugly...?
        Poco::Thread::sleep(50);
    };
    //This thread pool is done....
}


}
