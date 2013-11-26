#pragma hdrstop
#include "rrc_cpp_support.h"
#include "rrp_cpp_support.h"
#include "rrRoadRunnerData.h"
#include "rrException.h"
#include "rrStringUtils.h"

namespace rrp
{
using namespace rr;

Parameters* castToParameters(RRParametersHandle handle)
{
    Parameters* para = (Parameters*) handle;
    if(para) //Will only fail if handle is NULL...
    {
        return para;
    }
    else
    {
        Exception ex("Failed to cast to a valid Parameters handle");
        throw(ex);
    }
}

BaseParameter* castToParameter(RRParameterHandle handle)
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

Parameter<int>* castToIntParameter(RRParameterHandle handle)
{
    Parameter<int>* para = (Parameter<int>*) handle;
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

Parameter<double>* castToDoubleParameter(RRParameterHandle handle)
{
    Parameter<double>* para = (Parameter<double>*) handle;
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

Parameter<char*>* castToStringParameter(RRParameterHandle handle)
{
    Parameter<char*>* para = (Parameter<char*>*) handle;
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

RRCDataPtr createRRCData(const RoadRunnerData& result)
{
    return rrc::createRRCData(result);     
}


}