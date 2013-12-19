#pragma hdrstop
#include "rrc_cpp_support.h"
#include "rrp_cpp_support.h"
#include "rrRoadRunnerData.h"
#include "rrException.h"
#include "rrStringUtils.h"

namespace rrp
{
using namespace rr;

PluginManager* castToPluginManager(RRPluginManagerHandle handle)
{
    PluginManager *pm = static_cast<PluginManager*>(handle);
    if(pm)
    {
        return pm;
    }
    else
    {
        Exception ex("Failed to cast to a valid PluginManager handle");
        throw(ex);
    }
}

Plugin* castToPlugin(RRPluginHandle handle)
{
    Plugin* plugin = (Plugin*) handle;
    if(plugin) //Will only fail if handle is NULL...
    {
        return plugin;
    }
    else
    {
        Exception ex("Failed to cast to a valid Plugin handle");
        throw(ex);
    }
}

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

PluginParameter* castToParameter(RRParameterHandle handle)
{
    PluginParameter* para = (PluginParameter*) handle;
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

Parameter<bool>* castToBoolParameter(RRParameterHandle handle)
{
    Parameter<bool>* para = (Parameter<bool>*) handle;
    if(para) //Will only fail if handle is NULL...
    {
        return para;
    }
    else
    {
        Exception ex("Failed to cast to a valid Boolean Parameter handle");
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
        Exception ex("Failed to cast to a valid Integer Parameter handle");
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
        Exception ex("Failed to cast to a valid Double Parameter handle");
        throw(ex);
    }
}

Parameter<string>* castToStringParameter(RRParameterHandle handle)
{
    Parameter<string>* para = (Parameter<string>*) handle;
    if(para) //Will only fail if handle is NULL...
    {
        return para;
    }
    else
    {
        Exception ex("Failed to cast to a valid string Parameter handle");
        throw(ex);
    }
}

Parameter<Parameters>* castToParametersParameter(RRParameterHandle handle)
{
    Parameter<Parameters>* para = (Parameter<Parameters>*) handle;
    if(para) //Will only fail if handle is NULL...
    {
        return para;
    }
    else
    {
        Exception ex("Failed to cast to a valid string Parameter handle");
        throw(ex);
    }
}

RoadRunnerData* castToRoadRunnerData(RRDataHandle handle)
{
    RoadRunnerData* para = (RoadRunnerData*) handle;
    if(para) //Will only fail if handle is NULL...
    {
        return para;
    }
    else
    {
        Exception ex("Failed to cast to a valid string Parameter handle");
        throw(ex);
    }
}

Parameter<RoadRunnerData*>* castToRoadRunnerDataParameter(RRParameterHandle handle)
{
    Parameter<RoadRunnerData*>* para = (Parameter<RoadRunnerData*>*) handle;
    if(para) //Will only fail if handle is NULL...
    {
        return para;
    }
    else
    {
        Exception ex("Failed to cast to a valid string Parameter handle");
        throw(ex);
    }
}

RRCDataPtr createRRCData(const RoadRunnerData& result)
{
    return rrc::createRRCData(result);     
}


}