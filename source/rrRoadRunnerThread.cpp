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

}