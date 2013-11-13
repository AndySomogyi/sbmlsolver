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
//workerEnterCB(NULL),
//workerProgressCB(NULL),
//workerExitCB(NULL),
//mUserDataEnterCB(NULL),
//mUserDataProgressCB(NULL),
//mUserDataExitCB(NULL),
mTheHost(host),
mInputData(NULL)
{}

bool AddNoiseWorker::start(void* inputData, double sigma, bool runInThread)
{
    mInputData = inputData;
    mSigma = sigma;

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

    if(mInputData)
    {

        RoadRunnerData& data = *(RoadRunnerData*) (mInputData);
        Noise noise(0, mSigma);
        noise.randomize();

        for(int row = 0; row < data.rSize(); row++)
        {
            double xVal = data(row, 0);    //Time
            for(int col = 0; col < data.cSize() - 1; col++)
            {
                double yData = data(row, col + 1) + noise.getNoise();
                data(row, col + 1) = yData;
            }
            sleep(10);

            if(mTheHost.mWorkProgressCB)
            {
                mTheHost.mPluginProgress.setValue((double) (row /(data.rSize() -1.0)) *100.0);
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
