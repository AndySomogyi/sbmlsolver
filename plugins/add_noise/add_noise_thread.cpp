#pragma hdrstop
#include "rrSimulationData.h"
#include "rrLogger.h"
#include "add_noise_thread.h"
#include "rrNoise.h"
//---------------------------------------------------------------------------

using namespace rr;
AddNoiseThread::AddNoiseThread()
:
threadEnterCB(NULL),
threadExitCB(NULL),
mUserData(NULL)
{}

void AddNoiseThread::assignCallBacks(ThreadCB fn1, ThreadCB fn2, void* userData)
{
	threadEnterCB = fn1;
	threadExitCB = fn2;
    mUserData = userData;
}

void AddNoiseThread::start(void* inputData, double sigma)
{
	mInputData = inputData;
    mSigma = sigma;

    if(mThread.isRunning())
    {
    	Log(lError)<<"Tried to start a working thread!";
        return;
    }

	mThread.start(*this);
}

void AddNoiseThread::run()
{
	if(threadEnterCB)
    {
		threadEnterCB(mUserData);
    }

    SimulationData& data = *(SimulationData*) (mInputData);
	Noise noise(0, mSigma);

	for(int row = 0; row < data.rSize(); row++)
    {
        double xVal = data(row, 0);	//Time
        for(int col = 0; col < data.cSize() - 1; col++)
        {
            double yData = data(row, col + 1) + noise.GetNoise();

			data(row, col + 1) = yData;
        }
    }

	if(threadExitCB)
    {
		threadExitCB(mUserData);
    }
}
