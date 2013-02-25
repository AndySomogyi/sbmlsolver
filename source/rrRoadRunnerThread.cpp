#pragma hdrstop
#include "rrRoadRunnerThread.h"

using namespace Poco;
namespace rr
{

RoadRunnerThread::RoadRunnerThread() :
mIsTimeToDie(false)
{}

void RoadRunnerThread::setName(const string& name)
{
	mThread.setName(name);
}

string RoadRunnerThread::getName()
{
	return mThread.getName();
}

void RoadRunnerThread::exit()
{
	mIsTimeToDie = true;
    signalExit();
}


void RoadRunnerThread::start()
{
	mThread.start(*this);
}

void RoadRunnerThread::run()
{
	worker();
}

void RoadRunnerThread::join()
{
	mThread.join();
}

bool RoadRunnerThread::isActive()
{
	return mThread.isRunning();
}

void RoadRunnerThread::wait()
{
    //Check that all jobs been done
    //This should be checked in a thread, and using a condition Variable
    while(getNrOfJobsInQueue() > 0)
    {
        Sleep(50);
    };

	Sleep(50);	//We need a way to know if a thread is still doing processing...

    //Dispose thread..
    exit();

    //This could be checked in a thread, and using a condition Variable
    while(isActive() == true)
    {
	    exit(); //Ugly...?
        Sleep(50);
    };
    //This thread pool is done....
}

}