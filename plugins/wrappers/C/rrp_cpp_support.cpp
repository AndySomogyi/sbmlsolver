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

RRCDataPtr createRRCData(const RoadRunnerData& result)
{
    return rrc::createRRCData(result);     
}


}