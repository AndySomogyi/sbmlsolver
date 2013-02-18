#pragma hdrstop
#include "Poco/ScopedLock.h"
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
	mThread.getName();
}

void RoadRunnerThread::exit()
{
	mJobsCondition.signal();
	mIsTimeToDie = true;
}

void RoadRunnerThread::start()
{
	mThread.start(*this);
}

void RoadRunnerThread::run()	//This is called from runnable::start(this)
{
	worker();
}

void RoadRunnerThread::join()
{
	mThread.join();
}

void RoadRunnerThread::addJob(RoadRunner* rr)
{
	//getMutex
    Mutex::ScopedLock lock(mJobsMutex);
    mJobs.push_back(rr);
	mJobsCondition.signal();	//Tell the thread its time to go to work
}

unsigned int RoadRunnerThread::getNrOfJobsInQueue()
{
    Mutex::ScopedLock lock(mJobsMutex);
    return mJobs.size();
}

bool RoadRunnerThread::isRunning()
{
	return mThread.isRunning();
}

}