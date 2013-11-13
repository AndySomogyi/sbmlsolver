#pragma hdrstop
#include "rrRoadRunnerData.h"
#include "rrLogger.h"
#include "add_noise_worker.h"
#include "rrUtils.h"
#include "rrNoise.h"
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
        run(); //Not running worker in a thread!
    }
    return true;
}

bool AddNoiseWorker::isRunning()
{
    return mThread.isRunning();
}

void AddNoiseWorker::run()
{
    if(mTheHost.mWorkStartedCB)
    {
        mTheHost.mWorkStartedCB(mTheHost.mWorkStartedData);
    }

    if(mTheHost.mClientData)
    {
        RoadRunnerData& data = *(RoadRunnerData*) (mTheHost.mClientData);
        Noise noise(0, mTheHost.mSigma.getValue());
        noise.randomize();

        for(int row = 0; row < data.rSize(); row++)
        {
            for(int col = 0; col < data.cSize() - 1; col++)
            {
                double yData = data(row, col + 1) + noise.getNoise();
                data(row, col + 1) = yData;
            }

            sleep(30);

            if(mTheHost.mWorkProgressCB)
            {
                mTheHost.mPluginProgress.setValue( (int) 0.5 + (row /(data.rSize() -1.0)) *100.0);
                mTheHost.mWorkProgressCB((void*) mTheHost.mWorkProgressData);
            }
        }
    }

    if(mTheHost.mWorkFinishedCB)
    {
        mTheHost.mWorkFinishedCB(mTheHost.mWorkFinishedData);
    }
}
}
