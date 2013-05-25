#pragma hdrstop
#include "add_noise.h"
#include "rrRoadRunner.h"
#include "rrNoise.h"

//---------------------------------------------------------------------------
namespace addNoise
{
using namespace rr;

AddNoise::AddNoise(rr::RoadRunner* aRR)
:
Plugin(				    "AddNoise", 				"No Category", 			aRR),
mSelectNoiseType(	    "Select Noise Type", 		"selectNoiseType", 		"Select the noise type"),
mNoiseType(	       		"NoiseType", 				ntGaussian, 	 		"Noise Type"),
mAddGaussianNoise(	    "Add noise", 				"...", 					"Add Gaussian Noise in particular"),
mSigma(	    			"Sigma", 					1, 						"Sigma"),
mData(					"Data", 					NULL,					"Data to add noise to..")
{
	//Setup the plugins capabilities
    mSelectNoiseType.add(&mNoiseType);

	mAddGaussianNoise.add(&mSigma);
    mAddGaussianNoise.add(&mData);

    mCapabilities.push_back(mAddGaussianNoise);
    mCapabilities.push_back(mSelectNoiseType);
    mData.setValue(NULL);
}

AddNoise::~AddNoise()
{}

bool AddNoise::execute()
{
	pLog()<<"Executing the AddNoise plugin";

    rrc::RRResultHandle data = mData.getValue();

	Noise noise(0, mSigma.getValue());

	for(int row = 0; row < data->RSize; row++)
    {
        double xVal = data->Data[row*data->CSize];	//Time
        for(int col = 0; col < data->CSize - 1; col++)
        {
            double yData = data->Data[row*data->CSize + col + 1] + noise.GetNoise();

			data->Data[row*data->CSize + col + 1] = yData;
        }
    }

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
#endif




























extern "C" int _libmain(unsigned long reason)
{
	return 1;
}

