#pragma hdrstop
#include "rrLogger.h"
#include "add_noise.h"
#include "rrRoadRunner.h"
#include "rrNoise.h"

//---------------------------------------------------------------------------
namespace addNoise
{
using namespace rr;

AddNoise::AddNoise(rr::RoadRunner* aRR, WorkStartedCB fn1, WorkFinishedCB fn2)
:
Plugin(				    "AddNoise", 				"No Category", 			aRR, fn1, fn2),
mAddNoise(	   			"Add noise", 				"...", 					"Add Noise"),
mNoiseType(	       		"NoiseType", 				ntGaussian, 	 		"Noise Type"),
mSigma(	    			"Sigma", 					1, 						"Sigma"),
mAddNoiseThread()
{
	//Setup the plugins capabilities
    mCapabilities.add(mAddNoise);
    mAddNoise.addParameter(&mNoiseType);
	mAddNoise.addParameter(&mSigma);
}

AddNoise::~AddNoise()
{}

bool AddNoise::execute(void* inputData)
{
	Log(lDebug)<<"Executing the AddNoise plugin";

    //go away and carry out the work in a thread
    //Assign callback functions to communicate the progress of the thread
	mAddNoiseThread.assignCallBacks(mWorkStartedCB, mWorkFinishedCB, mUserData);
    mAddNoiseThread.start(inputData, mSigma.getValue());
	return true;
}

// Plugin factory function
rr::Plugin* __stdcall createPlugin(rr::RoadRunner* aRR)
{
    //allocate a new object and return it
	return new AddNoise(aRR);
}
}

#if defined(CG_UI)
    #if defined(STATIC_BUILD)
    	#pragma comment(lib, "roadrunner-static.lib")
    #else
    	#pragma comment(lib, "roadrunner.lib")
    #endif

#pragma comment(lib, "poco_foundation-static.lib")
#endif




























extern "C" int _libmain(unsigned long reason)
{
	return 1;
}

