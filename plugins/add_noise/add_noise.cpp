#pragma hdrstop
#include "rrLogger.h"
#include "add_noise.h"
#include "rrRoadRunner.h"
#include "rrNoise.h"

//---------------------------------------------------------------------------
namespace addNoise
{
using namespace rr;

AddNoise::AddNoise(rr::RoadRunner* aRR, PluginCallBackFnc fn1, PluginCallBackFnc fn2, PluginCallBackFnc fn3)
:
CPPPlugin(                 "AddNoise",                 "Signal Processing",    aRR, NULL),
mAddNoise(                 "Add noise",                 "",                             "Add Noise"),
mNoiseType(                "NoiseType",                 ntGaussian,                     "Type of noise (Gaussian..)."),
mSigma(                    "Sigma",                     1,                              "Size of applied noise"),
//mPluginProgress(           "Progress",                  0,                              "Indicate progress of plugin work in %"),
mAddNoiseWorker(*this)
{
    //Setup the plugins capabilities
    mCapabilities.add(mAddNoise);
    mAddNoise.addParameter(&mNoiseType);
    mAddNoise.addParameter(&mSigma);
//    mAddNoise.addParameter(&mPluginProgress);
}

AddNoise::~AddNoise()
{}

bool AddNoise::isWorking()
{
    return mAddNoiseWorker.isRunning();
}

bool AddNoise::execute(void* inputData, bool inThread)
{
    Log(lDebug)<<"Executing the AddNoise plugin by Totte Karlsson";

    //Capture data handle
    mClientData = inputData;

    //go away and carry out the work in a thread
    return mAddNoiseWorker.start(inThread);
}

// Plugin factory function
Plugin* plugins_cc createPlugin(rr::RoadRunner* aRR)
{
    //allocate a new object and return it
    return new AddNoise(aRR);
}

const char* plugins_cc getImplementationLanguage()
{
    return "CPP";
}

}



