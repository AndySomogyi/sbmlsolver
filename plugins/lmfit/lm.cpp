#pragma hdrstop
#include "rrLogger.h"
#include "lm.h"
#include "../../Wrappers/C/rrc_api.h"
#include "../../Wrappers/C/rrc_support.h"
#include "rrRoadRunner.h"
#include "rrRoadRunnerData.h"
#include "rrUtils.h"
//---------------------------------------------------------------------------

using namespace rr;
using namespace rrc;

LM::LM(rr::RoadRunner* aRR)
:
Plugin(				    "Levenberg-Marquardt Minimization",   			"No Category", 		aRR),
mLMFit(	 	  	 		"LMFit", 					"", 				"Run a one species fit"),	//The 'capability'
mTempFolder(			"TempFolder", 	  			"",  				"Tempfolder used in the fitting"),
mSBML(	    			"SBML", 					"<none>",			"SBML, i.e. the model to be used in the fitting"),
mMinimizationData(		"MinData", 					NULL,			"Data structure holding minimization data"),
mLMFitThread(*this)
{
	//Setup the plugins capabilities
    mLMFit.addParameter(&mTempFolder);

    mLMFit.addParameter(&mSBML);
    mLMFit.addParameter(&mMinimizationData);
    mCapabilities.add(mLMFit);
}

LM::~LM()
{

}

MinimizationResult*	LM::getMinimizationData()
{
	return mMinimizationData.getValue();
}

string LM::getTempFolder()
{
	return mTempFolder.getValue();
}

string LM::getSBML()
{
	return mSBML.getValue();
}

string LM::getResult()
{
//	mResult<<"Parameters: ";
//    string para = mParameterToFit.asString();
//
//    mResult<<para;
//    mResult<<"\n";
//	return mResult.getReport();
	return "";
}

bool LM::execute(void* inputData)
{
   	Log(lInfo)<<"Executing the LM plugin";
//	mResult <<"LM was started on: "<<getDateTime() <<"\n";
    //go away and carry out the work in a thread
    //Assign callback functions to communicate the progress of the thread
	mLMFitThread.assignCallBacks(mWorkStartedCB, mWorkFinishedCB, mUserData);

    RoadRunnerData& data = *(RoadRunnerData*) (inputData);
    mLMFitThread.start(&data);
	return true;
}

// Plugin factory function
rr::Plugin* __stdcall createPlugin(rr::RoadRunner* aRR)
{
    //allocate a new object and return it
	return new LM(aRR);
}


#if defined(CG_UI)
    #if defined(STATIC_BUILD)
//    	#pragma comment(lib, "roadrunner-static.lib")
    #else
    	#pragma comment(lib, "rrc_api.lib")
    	#pragma comment(lib, "roadrunner.lib")
    	#pragma comment(lib, "poco_foundation-static.lib")
    	#pragma comment(lib, "rr-libstruct-static.lib")
    #endif
#endif


