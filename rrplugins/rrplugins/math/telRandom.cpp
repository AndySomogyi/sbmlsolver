#pragma hdrstop
#include "telRandom.h"
#include "third_party/mtrand.h"
//---------------------------------------------------------------------------

namespace tlp
{

Random::Random(unsigned long seed)
:
mRand(seed) {}

double Random::next() const
{
    return mRand();
}
}

