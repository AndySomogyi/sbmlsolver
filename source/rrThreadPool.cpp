#pragma hdrstop
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
        return val;
    }
    return true;
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
    //This could be checked in a thread, and using a condition Variable
    while(isJobQueueEmpty() == false)
    {
//        Sleep(100);
    };

//	Sleep(20);	//We need a way to know if a thread is still doing processing...

    //Dispose threads..
    exitAll();

    //This could be checked in a thread, and using a condition Variable
    while(isActive() == true)
    {
//        Sleep(100);
    };
    //This thread pool is done....
}


}
