#pragma hdrstop
#include "rrLogger.h"
#include "add_noise.h"
#include "rrRoadRunner.h"
#include "noise.h"
#include "add_noise_docs.h"

//---------------------------------------------------------------------------
namespace addNoise
{
using namespace rr;

AddNoise::AddNoise(rr::RoadRunner* aRR, PluginCallBackFnc fn1, PluginCallBackFnc fn2, PluginCallBackFnc fn3)
:
CPPPlugin(                 "AddNoise",                 "Signal Processing",    aRR, NULL),
mAddNoise(                 "Add Noise",                 "Add artificial noise to data"),
mNoiseType(                "NoiseType",                 ntGaussian,                     "Type of noise (Gaussian = 0, Psychological = 1)."),
mSigma(                    "Sigma",                     1,                              "Size of applied noise"),
mAddNoiseWorker(*this)
{
    mVersion = "1.0";
    //Setup the plugins capabilities
    mCapabilities.add(mAddNoise);
    mAddNoise.addParameter(&mNoiseType);
    mAddNoise.addParameter(&mSigma);
}

AddNoise::~AddNoise()
{}

unsigned char* AddNoise::getManualAsPDF() const
{
    return pdf_doc;
}

unsigned int AddNoise::getPDFManualByteSize()
{
    return sizeofPDF;
}

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



