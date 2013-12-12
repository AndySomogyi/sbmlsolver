#pragma hdrstop
#include "rrRandom.h"
#include "mtrand.h"



namespace rr
{
Random::Random() {}

double Random::NextDouble() const
{
    return mRand();
}
}

