#ifndef full_space_minimizationH
#define full_space_minimizationH
#include "rrCapability.h"
#include "rrParameter.h"
#include "rrPlugin.h"

//---------------------------------------------------------------------------
namespace fullSpaceFit
{
using namespace rr;

class FullSpaceMinimization : public Plugin
{
	private:
    	Capability				mOneParameterFit;
		Parameter<string> 		mParameterToFit;
    	Parameter<int>			mMaxNrOfIterations;
		Parameter<string>		mDataToFitFileName;
    	Capability				mOneParameterFitResult;
   		Parameter<double>		mChiSquare;
        Parameter<double*>		mData;


    public:
    							FullSpaceMinimization(rr::RoadRunner* aRR = NULL);
					   		   ~FullSpaceMinimization();
		bool					execute();

};


extern "C"
{
#define EXP_FUNC __declspec(dllexport)
EXP_FUNC rr::Plugin* __stdcall	createPlugin(rr::RoadRunner* aRR);

}

}
#endif
