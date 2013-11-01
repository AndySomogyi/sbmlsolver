
#pragma hdrstop
#include "rrLogger.h"
#include "add_noise.h"
#include "rrRoadRunner.h"
#include "rrNoise.h"

//---------------------------------------------------------------------------
extern "C" const char* rrCallConv getImplementationLanguage();
namespace addNoise
{

using namespace rr;

AddNoise::AddNoise(rr::RoadRunner* aRR, WorkStartedCB fn1, WorkFinishedCB fn2)
:
Plugin(                    "AddNoise",                 "Signal Processing",    aRR, fn1, fn2),
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

string AddNoise::getImplementationLanguage()
{
    return "";//::getImplementationLanguage();
}

bool AddNoise::isWorking()
{
    return mAddNoiseThread.isRunning();
}

bool AddNoise::execute(void* inputData)
{
    Log(lDebug)<<"Executing the AddNoise plugin by Totte Karlsson";

    //go away and carry out the work in a thread
    //Assign callback functions to communicate the progress of the thread
    mAddNoiseThread.assignCallBacks(mWorkStartedCB, mWorkFinishedCB, mUserData);
    mAddNoiseThread.start(inputData, mSigma.getValue());
    return true;
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




