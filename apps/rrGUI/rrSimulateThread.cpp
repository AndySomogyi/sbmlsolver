#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include "MainForm.h"
#include "rrLogger.h"
#include "rrRoadRunner.h"
#include "rrSimulateThread.h"
#include "rrException.h"
#include "rrSimulationData.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

namespace rr
{
SimulateThread::SimulateThread(RoadRunner* rr, TMForm* mainForm)
:
mRR(rr),
mHost(mainForm)
{}

SimulateThread::~SimulateThread()
{}

void SimulateThread::AssignRRInstance(RoadRunner* rr)
{
	mRR = rr;
}

void SimulateThread::Worker()
{
	mIsStarted = true;
	mIsRunning = true;
    int nrOfSimulations = 0;
    while(!mIsTimeToDie)
    {
        if(mHost)
        {
        	if(!mHost->mData)
            {
                mHost->mData = new SimulationData;
                try
                {
                	mRR->simulateEx(0, 1+ nrOfSimulations, 1000);
                    nrOfSimulations++;
                    *(mHost->mData) = mRR->GetSimulationResult();
                }
                catch(const RRException& e)
                {
                    Log(lInfo)<<"Exception in RoadRunner Worker: "<<e.what();
                }

                TThread::Synchronize(NULL, mHost->PlotFromThread);
		        Log(lInfo)<<"Simulation number: "<<nrOfSimulations;
            }
        }

    }
	Log(lInfo)<<"Simulate thread is exiting";
	mIsRunning = false;
    mIsFinished = true;
}

}
