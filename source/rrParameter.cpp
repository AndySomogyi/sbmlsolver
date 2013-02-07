#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include "rrStringUtils.h"
#include "rrParameter.h"
//---------------------------------------------------------------------------

namespace rr
{

template<>
string Parameter<double>::getValueAsString() const
{
    return ToString(mValue, "%G");
}

template<>
string Parameter<int>::getValueAsString() const
{
    return ToString(mValue);
}

}
