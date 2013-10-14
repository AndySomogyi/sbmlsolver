#pragma hdrstop
#include "rrRandom.h"
//---------------------------------------------------------------------------



namespace rr
{
Random::Random() {}

double Random::NextDouble() const
{
	return mRand();
}
}

