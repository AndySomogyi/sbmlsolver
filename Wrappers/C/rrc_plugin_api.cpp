#pragma hdrstop
#include <sstream>
#include "rrParameter.h"
#include "rrRoadRunner.h"
#include "rrPluginManager.h"
#include "rrPlugin.h"
//#include "rrCModelGenerator.h"
#include "rrLogger.h"           //Might be useful for debugging later on
#include "rrc_api.h"
#include "rrException.h"
#include "rrUtils.h"
#include "rrStringUtils.h"
#include "rrc_macros.h"
#include "rrc_cpp_support.h"
//---------------------------------------------------------------------------

namespace rrc
{
using namespace std;
using namespace rr;

//PLUGIN Functions
bool rrCallConv loadPlugins(RRHandle handle)
{
	try
    {
        RoadRunner* rri = castFrom(handle);
    	return rri->getPluginManager().load();
    }
    catch_bool_macro
}

bool rrCallConv unLoadPlugins(RRHandle handle)
{
	try
    {
        RoadRunner* rri = castFrom(handle);
    	return rri->getPluginManager().unload();
    }
    catch_bool_macro
}

int rrCallConv getNumberOfPlugins(RRHandle handle)
{
	try
    {
        RoadRunner* rri = castFrom(handle);
    	return rri->getPluginManager().getNumberOfPlugins();
    }
    catch_int_macro
}

RRPluginHandle rrcCallConv getPlugin(RRHandle handle, const char* pluginName)
{
	try
    {
        RoadRunner* rr = castFrom(handle);
        Plugin* aPlugin = rr->getPluginManager().getPlugin(pluginName);
        return aPlugin;
    }
    catch_ptr_macro
}

RRStringArray* rrCallConv getPluginNames(RRHandle handle)
{
	try
    {
        RoadRunner* rri = castFrom(handle);
        StringList names = rri->getPluginManager().getPluginNames();
        return createList(names);
    }
    catch_ptr_macro
}

char* rrCallConv getPluginName(RRPluginHandle handle)
{
	try
    {
        Plugin* aPlugin = castToPlugin(handle);
        return createText(aPlugin->getName());
    }
    catch_ptr_macro
}

RRStringArray* rrCallConv getPluginCapabilities(RRPluginHandle handle)
{
	try
    {
        Plugin* aPlugin = castToPlugin(handle);
        if(aPlugin)
        {
        	StringList aList;
            Capabilities* caps = aPlugin->getCapabilities();
            if(!caps)
            {
            	return NULL;
            }

            for(int i = 0; i < caps->count(); i++)
            {
            	aList.add((*caps)[i]->getName());
            }
        	return createList(aList);
        }
        else
        {
	        return NULL;
        }
    }
	catch_ptr_macro
}

RRStringArray* rrCallConv getPluginParameters(RRPluginHandle handle, const char* capability)
{
	try
    {
        Plugin* aPlugin = castToPlugin(handle);
        if(aPlugin)
        {
        	StringList aList;
            Parameters* paras = aPlugin->getParameters(capability);
            if(!paras)
            {
            	return NULL;
            }

            for(int i = 0; i < paras->count(); i++)
            {
            	aList.add((*paras)[i]->getName());
            }
        	return createList(aList);
        }
        else
        {
	        return NULL;
        }
    }
	catch_ptr_macro
}

RRParameterHandle rrCallConv getPluginParameter(RRPluginHandle handle, const char* parameterName, const char* capabilitiesName)
{
	try
    {
        Plugin* aPlugin = castToPlugin(handle);
        rr::BaseParameter *para = NULL;
        if(aPlugin)
        {
			if(capabilitiesName != NULL)
            {
				para = aPlugin->getParameter(parameterName, capabilitiesName);
            }
            else
            {
            	para = aPlugin->getParameter(parameterName);
            }
       		return para;

        }
        return NULL;

    }
	catch_ptr_macro
}

bool rrCallConv setPluginParameter(RRPluginHandle handle, const char* parameterName, const char* value)
{
	try
    {
        Plugin* aPlugin = castToPlugin(handle);
        BaseParameter* aParameter = (BaseParameter*) getPluginParameter(aPlugin, parameterName, NULL);
        return setParameter(aParameter, value);
    }
	catch_bool_macro
}

char* rrCallConv getPluginInfo(RRPluginHandle handle)
{
	try
    {
        Plugin* aPlugin = castToPlugin(handle);
       	return createText(aPlugin->getInfo());
    }
    catch_ptr_macro
}

bool rrCallConv executePlugin(RRPluginHandle handle)
{
	return executePluginEx(handle, NULL);
}

bool rrCallConv executePluginEx(RRPluginHandle handle, void* userData)
{
	try
    {

        Plugin* aPlugin = castToPlugin(handle);
        return (aPlugin) ? aPlugin->execute(userData) : false;
    }
    catch_bool_macro
}

bool rrCallConv assignCallbacks(RRPluginHandle handle, pluginCallback cb1, pluginCallback cb2, void* userData)
{
	try
    {
        Plugin* aPlugin = castToPlugin(handle);
        return (aPlugin) ? aPlugin->assignCallbacks(cb1, cb2, userData) : false;
    }
    catch_bool_macro
}

char* rrcCallConv getPluginResult(RRPluginHandle handle)
{
	try
    {
        Plugin* aPlugin = castToPlugin(handle);
        return createText(aPlugin->getResult());
    }
    catch_ptr_macro
}

bool rrCallConv setInputData(RRPluginHandle handle, void* data)
{
	try
    {
        Plugin* aPlugin = castToPlugin(handle);
        return (aPlugin) ? aPlugin->setInputData(data) : false;
    }
    catch_bool_macro
}

bool rrcCallConv resetPlugin(RRPluginHandle handle)
{
	try
    {
        Plugin* aPlugin = castToPlugin(handle);

        return aPlugin->resetPlugin();
    }
    catch_bool_macro
}

}
