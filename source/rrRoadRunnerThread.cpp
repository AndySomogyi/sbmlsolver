//---------------------------------------------------------------------------

#pragma hdrstop
#include "rrRoadRunnerThread.h"

namespace rr
{
RoadRunnerThread::RoadRunnerThread(RoadRunner* rr) : mRR(rr)
{}

void RoadRunnerThread::run()	//This is called from runnable::start(this)
{
	worker();
}

void RoadRunnerThread::join()
{
	mThread.join();
}

bool RoadRunnerThread::isRunning()
{
	return mThread.isRunning();
}

}