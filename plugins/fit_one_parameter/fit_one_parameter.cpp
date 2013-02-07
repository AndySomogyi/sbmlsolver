#pragma hdrstop
#include "fit_one_parameter.h"
#include "rrRoadRunner.h"

#if defined(CG_UI)
    #if defined(STATIC_BUILD)
    	#pragma comment(lib, "roadrunner-static.lib")
    #else
    	#pragma comment(lib, "roadrunner.lib")
    #endif
#endif
//---------------------------------------------------------------------------
namespace fitOne
{
using namespace rr;

FitOneParameter::FitOneParameter(rr::RoadRunner* aRR)
:
Plugin("FitOneParameter", "No Category", aRR),
mOneParameterFit("OneParameterFit", "Run", "Runs a one parameter fit"),
mNrOfIterations(	"Number of Iterations", 	10,   	"Number of Iterations"),
mChiSquare(			"ChiSquare", 			   	0.123, 	"ChiSquare")
{
	//Setup the plugins capabilities
    mOneParameterFit.add(&mNrOfIterations);
	mOneParameterFit.add(&mChiSquare);
    mCapabilities.push_back(mOneParameterFit);
}

FitOneParameter::~FitOneParameter()
{}

bool FitOneParameter::execute()
{

	Log()<<"Executing the FitOneParameter plugin";
    //Create a fitting thread, start it and then return..

    for(int i = 0; i < mNrOfIterations.getValue(); i++)
    {
    	mRR->simulate();
    }

    //
	return true;
}

// Plugin factory function
rr::Plugin* __stdcall createRRPlugin(rr::RoadRunner* aRR)
{
    //allocate a new object and return it
	return new FitOneParameter(aRR);
}

}

