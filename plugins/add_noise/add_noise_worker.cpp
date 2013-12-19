#pragma hdrstop
#include "rrRoadRunnerData.h"
#include "rrLogger.h"
#include "add_noise_worker.h"
#include "rrUtils.h"
#include "noise.h"
#include "add_noise.h"
//---------------------------------------------------------------------------

namespace addNoise
{

using namespace rr;
AddNoiseWorker::AddNoiseWorker(AddNoise& host)
:
mTheHost(host)
{}

bool AddNoiseWorker::start(bool runInThread)
{
    if(runInThread)
    {
        if(mThread.isRunning())
        {
            Log(Logger::LOG_ERROR)<<"Tried to start a working thread!";
            return false;
        }

        mThread.start(*this);
    }
    else
    {
        run(); //Not running worker in a thread
    }
    return true;
}

bool AddNoiseWorker::isRunning()
{
    return mThread.isRunning();
}

void AddNoiseWorker::run()
{
    if(mTheHost.mWorkStartedEvent)
    {
        mTheHost.mWorkStartedEvent(NULL, mTheHost.mWorkStartedData2);
    }

    if(mTheHost.mData.getValue())
    {
        RoadRunnerData& data = *(mTheHost.mData.getValue());
        Noise noise(0, mTheHost.mSigma.getValue());
        noise.randomize();

        for(int row = 0; row < data.rSize(); row++)
        {
            for(int col = 0; col < data.cSize() - 1; col++)
            {
                double yData = data(row, col + 1) + noise.getNoise();
                data(row, col + 1) = yData;
            }

            if(mTheHost.mWorkProgressEvent)
            {                
                int progress = (int) (row * 100.0) /(data.rSize() -1.0) ;
                mTheHost.mWorkProgressEvent((void*) &progress,  mTheHost.mWorkProgressData2);
            }
        }
    }
    

    if(mTheHost.mWorkFinishedEvent)
    {
        mTheHost.mWorkFinishedEvent(NULL, mTheHost.mWorkFinishedData2);
    }
}

}
