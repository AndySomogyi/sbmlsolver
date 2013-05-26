#ifndef add_noiseH
#define add_noiseH
#include "rrCapability.h"
#include "rrParameter.h"
#include "rrPlugin.h"
#include "rrc_types.h"
#include "add_noise_thread.h"
//---------------------------------------------------------------------------

namespace addNoise
{

using namespace rr;
using namespace rrc;

typedef void    (__stdcall *WorkStartedCB)(void*);
typedef void    (__stdcall *WorkFinishedCB)(void*);


class AddNoise : public Plugin
{
	public:
    	enum NoiseType {ntGaussian = 0};

	private:
    	Capability				mAddNoise;

		Parameter<NoiseType> 	mNoiseType;
        Parameter<double>		mSigma;
		AddNoiseThread			mAddNoiseThread;

    public:
    							AddNoise(RoadRunner* aRR = NULL, WorkStartedCB fn1 = NULL, WorkFinishedCB fn2 = NULL);
					   		   ~AddNoise();
		bool					execute(void* userData);
};

extern "C"
{
#define EXP_FUNC __declspec(dllexport)
EXP_FUNC rr::Plugin* __stdcall	createPlugin(rr::RoadRunner* aRR);
}

}

namespace rr
{
template<>
string Parameter<addNoise::AddNoise::NoiseType>::getType() const
{
	return "NoiseType";
}
template<>
string Parameter<addNoise::AddNoise::NoiseType>::getValueAsString() const
{
	return "Gaussian";
}

template<>
void Parameter< addNoise::AddNoise::NoiseType >::setValueFromString(const string& val)
{
	mValue = addNoise::AddNoise::ntGaussian;
}

}

#endif
