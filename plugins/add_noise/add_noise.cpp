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
CPPPlugin(                 "AddNoise",                 "Signal Processing",    aRR, fn1, fn2, fn3),
mAddNoise(                 "Add noise",                 "...",                          "Add Noise"),
mNoiseType(                "NoiseType",                 ntGaussian,                     "Type of Noise."),
mSigma(                    "Sigma",                     1,                              "Indicate the size of the noise"),
mAddNoiseThread()
{
    //Setup the plugins capabilities
    mCapabilities.add(mAddNoise);
    mAddNoise.addParameter(&mNoiseType);
    mAddNoise.addParameter(&mSigma);
}

AddNoise::~AddNoise()
{}

bool AddNoise::isWorking()
{
    return mAddNoiseThread.isRunning();
}

bool AddNoise::execute(void* inputData, bool runInThread)
{
    Log(lDebug)<<"Executing the AddNoise plugin by Totte Karlsson";
    mAddNoiseThread.assignCallBacks(mWorkStartedCB, mWorkFinishedCB, mUserData);

    //go away and carry out the work in a thread
    //Assign callback functions to communicate the progress of the thread
    return mAddNoiseThread.start(inputData, mSigma.getValue(), runInThread);
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

_xmlNode* AddNoise::createConfigNode()
{
    _xmlNode *cap = Configurable::createCapabilityNode("Add Noise", "", "Add Noise Plugin");
    Configurable::addChild(cap, Configurable::createParameterNode("Noise Type", "Noise Type", ntGaussian));
    Configurable::addChild(cap, Configurable::createParameterNode("Sigma", "Sigma", 1));
    return cap;
}

void AddNoise::loadConfig(const _xmlDoc* doc)
{}

}

extern "C" int _libmain(unsigned long reason)
{
    return 1;
}




