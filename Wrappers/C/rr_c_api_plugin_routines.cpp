
#pragma hdrstop
//---------------------------------------------------------------------------
#include <sstream>
#include "rrRoadRunner.h"
#include "rrPluginManager.h"
#include "rrPlugin.h"
#include "rrCGenerator.h"
#include "rrLogger.h"           //Might be useful for debugging later on
#include "rr_c_api.h"
#include "rr_c_api_support.h"   //Support functions, not exposed as api functions and or data
#include "rrException.h"
#include "rrUtils.h"
#include "rrStringUtils.h"

using namespace rr_c_api;
extern RoadRunner*     gRRHandle;

bool rrCallConv loadPlugins()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
        }

    	return gRRHandle->getPluginManager().load();
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
  	    return false;
    }
}

bool rrCallConv unLoadPlugins()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
        }

    	return gRRHandle->getPluginManager().unload();
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
  	    return false;
    }
}

int rrCallConv getNumberOfPlugins()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
        }

    	return gRRHandle->getPluginManager().getNumberOfPlugins();
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
  	    return -1;
    }
}

char* rrCallConv getPluginInfo(const char* name)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
        }

        Plugin* aPlugin = gRRHandle->getPluginManager().getPlugin(name);
        if(aPlugin)
        {
        	return createText(aPlugin->getInfo());
        }
        else
        {
	        return createText("No such plugin: " + string(name));
        }
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
  	    return NULL;
    }
}

bool rrCallConv executePlugin(const char* name)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
        }

        Plugin* aPlugin = gRRHandle->getPluginManager().getPlugin(name);
        if(aPlugin)
        {
        	return aPlugin->execute();
        }
        else
        {
	        return false;
        }
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
  	    return NULL;
    }
}
