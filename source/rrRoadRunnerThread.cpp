#pragma hdrstop
#include "Poco/ScopedLock.h"
#include "rrRoadRunnerThread.h"

using namespace Poco;
namespace rr
{

//static int 	mActiveCount = 0;

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
    //	mJobsCondition.signal();
}

//void RoadRunnerThread::signalAll()
//{
//	mJobsCondition.broadcast();
//}

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

//void RoadRunnerThread::addJob(RoadRunner* rr)
//{
//	//getMutex
//    Mutex::ScopedLock lock(mJobsMutex);
//    mJobs.push_back(rr);
//	mJobsCondition.signal();	//Tell the thread its time to go to work
//}
//
//unsigned int RoadRunnerThread::getNrOfJobsInQueue()
//{
//    Mutex::ScopedLock lock(mJobsMutex);
//    return mJobs.size();
//}

bool RoadRunnerThread::isActive()
{
	return mThread.isRunning();
}

}