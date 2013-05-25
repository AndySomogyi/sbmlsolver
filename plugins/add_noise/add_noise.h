#ifndef add_noiseH
#define add_noiseH
#include "rrCapability.h"
#include "rrParameter.h"
#include "rrPlugin.h"
#include "rrc_types.h"

//---------------------------------------------------------------------------
namespace addNoise
{
using namespace rr;
using namespace rrc;

class AddNoise : public Plugin
{
	public:
    	enum NoiseType {ntGaussian = 0 } ;

	private:
	    Capability				mSelectNoiseType;
		Parameter<NoiseType> 	mNoiseType;

    	Capability				mAddGaussianNoise;
        Parameter<double>		mSigma;
        Parameter<RRResult*>  	mData;

    public:
    							AddNoise(RoadRunner* aRR = NULL);
					   		   ~AddNoise();
		bool					execute();
};

extern "C"
{
#define EXP_FUNC __declspec(dllexport)
EXP_FUNC rr::Plugin* __stdcall	createPlugin(rr::RoadRunner* aRR);

}

}
#endif
