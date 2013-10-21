#pragma hdrstop
#include "rrp_cpp_support.h"
#include "rrException.h"

namespace rrp
{
using namespace rr;
BaseParameter* castToParameter(rrp::RRParameterHandle handle)
{
    BaseParameter* para = (BaseParameter*) handle;
    if(para) //Will only fail if handle is NULL...
    {
        return para;
    }
    else
    {
        Exception ex("Failed to cast to a valid Parameter handle");
        throw(ex);
    }
}

MinimizationData* castToMinimizationData(RRMinimizationDataHandle handle)
{
    MinimizationData* para = (MinimizationData*) handle;
    if(para) //Will only fail if handle is NULL...
    {
        return para;
    }
    else
    {
        Exception ex("Failed to cast to a valid MinimizationData handle");
        throw(ex);
    }
}

}