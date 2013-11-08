#pragma hdrstop
#include "rrRoadRunnerData.h"
#include "rrLogger.h"
#include "add_noise_thread.h"
#include "rrNoise.h"
//---------------------------------------------------------------------------

using namespace rr;
AddNoiseThread::AddNoiseThread()
:
threadEnterCB(NULL),
threadExitCB(NULL),
mUserData(NULL),
mInputData(NULL)
{}

void AddNoiseThread::assignCallBacks(ThreadCB fn1, ThreadCB fn2, void* userData)
{
    threadEnterCB = fn1;
    threadExitCB = fn2;
    mUserData = userData;
}

bool AddNoiseThread::start(void* inputData, double sigma, bool runInThread)
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

bool AddNoiseThread::isRunning()
{
    return mThread.isRunning();
}

void AddNoiseThread::run()
{
    if(threadEnterCB)
    {
        threadEnterCB(mUserData);
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
        }
    }

    if(threadExitCB)
    {
        threadExitCB(mUserData);
    }
}