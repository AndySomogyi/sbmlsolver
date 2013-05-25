#pragma hdrstop
#include "rrLogger.h"
#include "full_space_minimization.h"
#include "../../Wrappers/C/rrc_api.h"
#include "../../Wrappers/C/rrc_support.h"
#include "rrRoadRunner.h"
#include "rrSimulationData.h"
//---------------------------------------------------------------------------
namespace fullSpaceFit
{
using namespace rr;
using namespace rrc;

vector<string> tmp;
FullSpaceMinimization::FullSpaceMinimization(rr::RoadRunner* aRR)
:
Plugin(				    "FullSpaceMinimization",   	"No Category", 		aRR),
mOneParameterFit(	    "OneParameterFit", 			"Run", 				"Runs a one parameter fit"),
mTempFolder(			"TempFolder", 	  			"",  				"Tempfolder used in the fitting"),
mInputData(	   			"InputData", 				NULL,				"Input Data"),
mSBML(	    			"SBML", 					"<none>",			"SBML, i.e. the model to be used in the fitting"),
mStepsPerDimension(		"Steps per dimension",		10,					"Steps..."),
mNumberOfThreads(		"Nr of threads to use",		4,					"Number of threads to use"),
mParameterSweepRange(	"Parameter sweep range",	10,					"Sweep range in precent"),
mParametersToFit(	    "Parameters to fit", 		tmp,   				"Parameter to Fit"),
mResultData(	   	 	"ResultData", 				NULL,				"Output Data"),
mChiSquare(			    "ChiSquare", 			   	0.123, 				"ChiSquare"),
mFullSpaceFitThread(*this)
{
	//Setup the plugins capabilities
    mOneParameterFit.add(&mTempFolder);
    mOneParameterFit.add(&mInputData);
    mOneParameterFit.add(&mSBML);
	mOneParameterFit.add(&mStepsPerDimension);
    mOneParameterFit.add(&mNumberOfThreads);
    mOneParameterFit.add(&mParameterSweepRange);
    mOneParameterFit.add(&mParametersToFit);
	mOneParameterFit.add(&mChiSquare);

    mCapabilities.push_back(mOneParameterFit);
}

FullSpaceMinimization::~FullSpaceMinimization()
{}

string FullSpaceMinimization::getTempFolder()
{
	return mTempFolder.getValue();
}

string FullSpaceMinimization::getSBML()
{
	return mSBML.getValue();
}

vector<string> FullSpaceMinimization::getParametersToFit()
{
	return mParametersToFit.getValue();
}

bool FullSpaceMinimization::execute(void* inputData)
{
   	Log(lInfo)<<"Executing the FullSpaceMinimization plugin";

    SimulationData& data = *(SimulationData*) (inputData);

    //go away and carry out the work in a thread
    //Assign callback functions to communicate the progress of the thread
	mFullSpaceFitThread.assignCallBacks(mWorkStartedCB, mWorkFinishedCB, mUserData);
    mFullSpaceFitThread.start(&data);
	return true;
}

// Plugin factory function
rr::Plugin* __stdcall createPlugin(rr::RoadRunner* aRR)
{
    //allocate a new object and return it
	return new FullSpaceMinimization(aRR);
}

}

#if defined(CG_UI)
    #if defined(STATIC_BUILD)
    	#pragma comment(lib, "roadrunner-static.lib")
    #else
    	#pragma comment(lib, "rrc_api.lib")
    	#pragma comment(lib, "roadrunner.lib")

    	#pragma comment(lib, "poco_foundation-static.lib")
    	#pragma comment(lib, "rr-libstruct-static.lib")
    #endif
#endif


